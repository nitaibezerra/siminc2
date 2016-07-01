BEGIN TRANSACTION;

DECLARE @IDX, @IDX_REMOVIDO, @DUPLICADOS, @REMOVIDOS, @V_ANSID, @V_PRDID, @V_DSPID, @V_SFPID, @TOTAL_REMOVIDOS, @V_DTAID ;

BEGIN
SET @DUPLICADOS = SELECT ans.ansid, fdp.prdid, di.dspid, COUNT(*) as total
		  FROM fabrica.analisesolicitacao ans
		  LEFT JOIN fabrica.servicofaseproduto sfp
  		    ON ans.ansid = sfp.ansid
		  LEFT JOIN fabrica.fasedisciplinaproduto fdp
		    ON sfp.fdpid = fdp.fdpid
		  LEFT JOIN fabrica.fasedisciplina di
		    ON fdp.fsdid = di.fsdid  
		  GROUP BY ans.ansid, fdp.prdid, di.dspid
		  HAVING COUNT(*) > 1
		  ORDER BY ans.ansid, fdp.prdid,di.dspid;

SET @IDX = 0;
SET @TOTAL_REMOVIDOS = 0;

WHILE( @DUPLICADOS[@IDX])
BEGIN
  SET @V_ANSID = @DUPLICADOS[@IDX][0]; 
  SET @V_PRDID = @DUPLICADOS[@IDX][1]; 
  SET @V_DSPID = @DUPLICADOS[@IDX][2];
  
  SET @REMOVIDOS = SELECT sfp.sfpid
		   FROM fabrica.servicofaseproduto sfp
		   INNER JOIN fabrica.fasedisciplinaproduto fdp
		     ON sfp.fdpid = fdp.fdpid
		   INNER JOIN fabrica.fasedisciplina di
		     ON fdp.fsdid = di.fsdid
		   WHERE sfp.sfpid NOT IN ( 
					SELECT sfp.sfpid
					FROM fabrica.servicofaseproduto sfp
					INNER JOIN fabrica.fasedisciplinaproduto fdp
					   ON sfp.fdpid = fdp.fdpid
					INNER JOIN fabrica.fasedisciplina di
					   ON fdp.fsdid = di.fsdid
					WHERE sfp.ansid = @V_ANSID
					AND fdp.prdid 	= @V_PRDID
					AND di.dspid 	= @V_DSPID
					--ORDER BY fdp.prdid, di.dspid 
					LIMIT 1
					)
		   AND sfp.ansid = @V_ANSID
		   AND fdp.prdid = @V_PRDID
		   AND di.dspid	 = @V_DSPID ;
		   
  SET @IDX_REMOVIDO = 0;

  WHILE(@REMOVIDOS[@IDX_REMOVIDO] )
  BEGIN
    SET @V_SFPID = @REMOVIDOS[@IDX_REMOVIDO][0];
    
    DELETE FROM fabrica.itemauditoriadetalhesauditoria WHERE dtaid IN  (SELECT da.dtaid FROM fabrica.detalhesauditoria da WHERE da.sfpid = @V_SFPID );	
    DELETE FROM fabrica.detalhesauditoria da WHERE da.sfpid = @V_SFPID;	   
    DELETE FROM fabrica.servicofaseproduto sfp WHERE sfp.sfpid = @V_SFPID;
    
    SET @TOTAL_REMOVIDOS = @TOTAL_REMOVIDOS + 1;
    SET @IDX_REMOVIDO = @IDX_REMOVIDO + 1;
  END		   
		    
  SET @IDX = @IDX + 1;
END 

PRINT @TOTAL_REMOVIDOS;
END

COMMIT TRANSACTION;