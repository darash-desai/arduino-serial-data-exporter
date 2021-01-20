#ifndef SerialDataExporter_h
#define SerialDataExporter_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/**
 * Defines the number of different buffer types that are used to store variables
 * to export. Each buffer type reflects a buffer used to store a unique data
 * type (char, int, double, etc.), with an additional buffer for labels.
 */
const int NUM_BUFFER_TYPES = 3;

const int STRING_INDEX = 0;
const int INT_INDEX = 1;
const int DOUBLE_INDEX = 2;

class SerialDataExporter
{

public:
	/**
	 * Constructor.
	 *
	 * @param serialPort		Reference to the stream where data should be exported
	 *											to (e.g. Serial).
	 * @param	bufferSizes		The buffer sizes to use for each buffer type.
	 */
  SerialDataExporter(
      Stream &serialPort,
      int bufferSizes[NUM_BUFFER_TYPES]
  );

	/**
	 * Adds an integer to the export buffer.
	 *
	 * @param label				The label for the variable.
	 * @param variable		The variable value.
	 *
	 * @returns		True if the variable was added to the export buffer or false
	 *						if there is no space left.
	 */
  bool add(const char *label, int variable);

	/**
	 * Adds a double to the export buffer.
	 *
	 * @param label				The label for the variable.
	 * @param variable		The variable value.
	 *
	 * @returns		True if the variable was added to the export buffer or false
	 *						if there is no space left.
	 */
  bool add(const char *label, double variable);

	/**
	 * Exports the current buffer to Serial in JSON format.
	 *
	 * @param precision		Optional parameter indicating the output precision for
	 *										floating point data types.
	 */
	void exportJSON(int precision = 6);

	/**
	 * Exports the current buffer to Serial for use with the Arduino Serial
	 * plotter.
	 *
	 * @param precision		Optional parameter indicating the output precision for
	 *										floating point data types.
	 */
	void exportToPlotter(int precision = 6);

	/** Clears the current buffer of variables to export. */
	void clear();


private:
  /** Reference to the serial port to export data through. */
  Stream &m_serial;

  /** Reference to buffer for variable labels. */
  char *m_labels;

  /** Reference to buffer for int variables. */
  int *m_ints;

	/** Reference to buffer for double variables. */
  double *m_doubles;

	/** Stores the total size of each buffer type. */
  int m_bufferLengths[NUM_BUFFER_TYPES];

	/** Reference to current position within the each buffer. */
  int m_bufferPositions[NUM_BUFFER_TYPES];

	/** Stores the order variables are added and the data type for each. */
	byte *m_variableOrder;

	/** The current number of variables that have been stored. */
	int m_size = 0;

	/** Adds a label to the label buffer. */
	bool addLabel(const char *label);

};

#endif
