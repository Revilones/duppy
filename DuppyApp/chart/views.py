from django.shortcuts import render

# Create your views here.

def chart(request):
	context = []
	return render(request, 'chart/chart.html', context)

def newdata(request):
	# make new test data

	return chart(request)