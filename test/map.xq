for $rt in mapper/resultMap/result
return 
	fn:string-join((fn:string($rt/@column),fn:string($rt/@property),fn:string($rt/@type),fn:string($rt/@key_type)),'|')