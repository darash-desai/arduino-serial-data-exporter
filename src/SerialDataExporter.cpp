#include "SerialDataExporter.h"

SerialDataExporter::SerialDataExporter(
    Stream &serialPort,
    int bufferSizes[NUM_BUFFER_TYPES]
) :
  m_serial(serialPort),
  m_labels((char*) malloc(bufferSizes[STRING_INDEX] * sizeof(char))),
  m_ints((int*) malloc(bufferSizes[INT_INDEX] * sizeof(int))),
  m_doubles((double*) malloc(bufferSizes[DOUBLE_INDEX] * sizeof(double)))
{
  int totalLength = 0;
  for (int index = 0; index < NUM_BUFFER_TYPES; index++) {
    m_bufferLengths[index] = bufferSizes[index];
    m_bufferPositions[index] = 0;

    // Skip first position refering the label buffer
    if (index > 0) {
      totalLength += m_bufferLengths[index];
    }
  }

  m_variableOrder = (byte*) malloc(totalLength * sizeof(byte));
}

bool SerialDataExporter::add(const char *label, int variable) {
  if(m_bufferPositions[INT_INDEX] >= m_bufferLengths[INT_INDEX] -1) return false;
  if(!addLabel(label)) return false;

  m_ints[m_bufferPositions[INT_INDEX]] = variable;
  m_bufferPositions[INT_INDEX]++;
  m_variableOrder[m_size] = INT_INDEX;
  m_size++;

  return true;
}

bool SerialDataExporter::add(const char *label, double variable) {
  if(m_bufferPositions[DOUBLE_INDEX] >= m_bufferLengths[DOUBLE_INDEX] -1) return false;
  if(!addLabel(label)) return false;

  m_doubles[m_bufferPositions[DOUBLE_INDEX]] = variable;
  m_bufferPositions[DOUBLE_INDEX]++;
  m_variableOrder[m_size] = DOUBLE_INDEX;
  m_size++;

  return true;
}

void SerialDataExporter::exportJSON(int precision) {
  Serial.print("{");
  // Reset all of the buffer position values so that the array may be used to
  // track which position is to be printed next for that buffer type in the
  // export logic below.
  for (int index = 0; index < NUM_BUFFER_TYPES; index++) {
    m_bufferPositions[index] = 0;
  }

  char *labelPosition;
  byte variableBuffer;
  for (int counter = 0; counter < m_size; counter++) {
    variableBuffer = m_variableOrder[counter];
    labelPosition = m_labels + m_bufferPositions[STRING_INDEX];

    Serial.print("\"");
    Serial.print(labelPosition);
    Serial.print("\"");
    Serial.print(":");

    switch(variableBuffer) {
      case INT_INDEX:
        Serial.print(m_ints[m_bufferPositions[INT_INDEX]]);
        break;

      case DOUBLE_INDEX:
        Serial.print(m_doubles[m_bufferPositions[DOUBLE_INDEX]], precision);
        break;
    }

    if (counter != m_size -1) {
      Serial.print(",");
    }

    m_bufferPositions[STRING_INDEX] += strlen(labelPosition) + 1;
    m_bufferPositions[variableBuffer]++;
  }

  // Reset the buffer data to prepare for a new set of export data
  clear();
  Serial.println("}");
}

void SerialDataExporter::exportToPlotter(int precision) {
  // Reset all of the buffer position values so that the array may be used to
  // track which position is to be printed next for that buffer type in the
  // export logic below.
  for (int index = 0; index < NUM_BUFFER_TYPES; index++) {
    m_bufferPositions[index] = 0;
  }

  char *labelPosition;
  byte variableBuffer;
  for (int counter = 0; counter < m_size; counter++) {
    variableBuffer = m_variableOrder[counter];
    labelPosition = m_labels + m_bufferPositions[STRING_INDEX];

    Serial.print(labelPosition);
    Serial.print(":");

    switch(variableBuffer) {
      case INT_INDEX:
        Serial.print(m_ints[m_bufferPositions[INT_INDEX]]);
        break;

      case DOUBLE_INDEX:
        Serial.print(m_doubles[m_bufferPositions[DOUBLE_INDEX]], precision);
        break;
    }

    if (counter != m_size -1) {
      Serial.print(",");
    }

    m_bufferPositions[STRING_INDEX] += strlen(labelPosition) + 1;
    m_bufferPositions[variableBuffer]++;
  }

  // Reset the buffer data to prepare for a new set of export data
  clear();

  Serial.println();
}

void SerialDataExporter::clear() {
  m_size = 0;
  for (int index = 0; index < NUM_BUFFER_TYPES; index++) {
    m_bufferPositions[index] = 0;
  }
}

bool SerialDataExporter::addLabel(const char *label) {
  int length = strlen(label) + 1;
  if (m_bufferPositions[STRING_INDEX] + length > m_bufferPositions[STRING_INDEX] + m_bufferLengths[STRING_INDEX]) {
    return false;
  }

  memcpy(m_labels + m_bufferPositions[STRING_INDEX], label, length);
  m_bufferPositions[STRING_INDEX] += length;

  return true;
}
