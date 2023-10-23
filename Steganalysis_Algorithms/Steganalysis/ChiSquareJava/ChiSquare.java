import java.awt.image.BufferedImage;
import java.awt.Color;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import org.apache.commons.math3.stat.inference.ChiSquareTest;

public class ChiSquare{

	public static void chiSquareAttackTopToBottom(BufferedImage image, double[] x, double[] chi, int size)
	{
		int width = image.getWidth();
		int height = image.getHeight();
		int block = 0;
		int nbBytes = 1;
		int red, green, blue;
		int[] values = new int[256];
		double[] expectedValues = new double[128];
		long[] pov = new long[128];
		
		for(int i=0; i<values.length; i++)
		{
			values[i] = 1;
			x[i] = i;
		}

		for(int j=0; j<height; j++)
		{
			for(int i=0; i<width; i++)
			{
				if(block < chi.length)
				{	
					red = (new Color(image.getRGB(i, j))).getRed();
					values[red]++;
					nbBytes++;
					if(nbBytes > size)
					{
						for(int k=0; k<expectedValues.length; k++)
						{
							expectedValues[k] = (values[2*k]+values[2*k+1])/2;
							pov[k] = values[2*k];
						}
						chi[block] = new ChiSquareTest().chiSquareTest(expectedValues, pov);
						block++;
						nbBytes = 1;
						/*for(int m=0; m<values.length; m++)
						{
							values[m] = 1;
						}*/
					}
				}
				
				if(block < chi.length)
				{
					green = (new Color(image.getRGB(i, j))).getGreen();
					values[green]++;
					nbBytes++;
					if(nbBytes > size)
					{
						for(int k=0; k<expectedValues.length; k++)
						{
							expectedValues[k] = (values[2*k]+values[2*k+1])/2;
							pov[k] = values[2*k];
						}
						chi[block] = new ChiSquareTest().chiSquareTest(expectedValues, pov);
						block++;
						nbBytes = 1;
						/*for(int m=0; m<values.length; m++)
						{
							values[m] = 1;
						}*/
					}
				}

				if(block < chi.length)
				{
					blue = (new Color(image.getRGB(i, j))).getBlue();
					values[blue]++;			
					nbBytes++;
					if(nbBytes > size)
					{
						for(int k=0; k<expectedValues.length; k++)
						{
							expectedValues[k] = (values[2*k]+values[2*k+1])/2;
							pov[k] = values[2*k];
						}
						chi[block] = new ChiSquareTest().chiSquareTest(expectedValues, pov);
						block++;
						nbBytes = 1;
						/*for(int m=0; m<values.length; m++)
						{
							values[m] = 1;
						}*/
					}
				}
			}
		}
	}

  public static void main(String[] args){
	BufferedImage image = null;

	try{
		image = ImageIO.read(new File("../stego_img_flwr.jpeg"));
	} catch(IOException e){
		System.out.println("Exception occured :" + e.getMessage());
	}
    
	int size = 256;
	double[] x = new double[size];
	double[] chi = new double[size];

	chiSquareAttackTopToBottom(image, x, chi, size);

	for(int i = 0; i < size; i++){
		System.out.print(chi[i] + " ");
	}System.out.println();
  }
}
