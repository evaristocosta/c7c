.PHONY: clean All

All:
	@echo "----------Building project:[ C7 - Release ]----------"
	@"$(MAKE)" -f  "C7.mk"
clean:
	@echo "----------Cleaning project:[ C7 - Release ]----------"
	@"$(MAKE)" -f  "C7.mk" clean
