import scrapy

class MySpider(scrapy.Spider):
    name = 'myspider'
    start_urls = ['http://example.com']

    def parse(self, response):
        title = response.xpath('//title/text()').extract_first()
        print(title)