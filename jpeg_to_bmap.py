from PIL import Image

def read_image(file_path):
    print("Extracting...")
    # Open the image file
    img = Image.open(file_path)

    # Get the dimensions of the image
    width, height = img.size

    # Create a matrix to store the RGB values
    pixel_matrix = [[0] * width for _ in range(height)]

    # Read pixels and store in the matrix
    for y in range(height):
        for x in range(width):
            # Get the RGB values at the current pixel
            pixel_value = img.getpixel((x, y))
            # Store the RGB values in the matrix
            pixel_matrix[y][x] = pixel_value
    print("Done")
    return pixel_matrix

def write_matrix_to_text(matrix, output_file):
    print("Storing...")
    with open(output_file, 'w') as file:
        file.write(f"{len(matrix)}")
        file.write(' ')
        file.write(f"{len(matrix[0])}")
        file.write('\n')
        for row in matrix:
            for rgb_values in row:
                # Write RGB values to the file
                file.write(f"{rgb_values} ")
            file.write('\n')
    print("Done")

write_matrix_to_text(read_image('resources/metal.jpg'), 'resources/metal.bmap')