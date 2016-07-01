BEGIN TRANSACTION;

DECLARE @V_TPDID, @V_ESDID, @V_DOCID, @V_SOLICITAOSERVICO { @ANSID }, @V_PREPOSTO_SQUADRA, @V_ANSID, @V_AUDID, @V_AUD_DOCID;
    
BEGIN           
  
    SET @V_TPDID = SELECT  tpd.tpdid
    FROM workflow.tipodocumento tpd
    WHERE tpd.tpddsc 	= 'Artefatos'
    AND tpd.tpdstatus 	= 'A';
    
    SET @V_ESDID = SELECT ed.esdid 
    FROM workflow.estadodocumento ed
    WHERE ed.tpdid = @V_TPDID
    AND ed.esddsc = 'Pendente';

    SET @V_SOLICITAOSERVICO = SELECT ans.ansid, aud.audid, ss.scsid, aud.docid
			      FROM fabrica.solicitacaoservico ss
			      INNER JOIN fabrica.analisesolicitacao ans
			        ON ss.scsid = ans.scsid
			      LEFT JOIN fabrica.auditoria aud
				ON ans.ansid = aud.ansid;    

    SET @V_TPDID 		= @V_TPDID[0][0];
    SET @V_ESDID 		= @V_ESDID[0][0];    
    SET @IDX = 0;

    WHILE @V_SOLICITAOSERVICO[@IDX]
    BEGIN
      
      SET @V_ANSID = @V_SOLICITAOSERVICO[@IDX][0];   
      SET @V_AUDID = @V_SOLICITAOSERVICO[@IDX][1];
      SET @V_SCSID = @V_SOLICITAOSERVICO[@IDX][2];   
      SET @V_AUD_DOCID = @V_SOLICITAOSERVICO[@IDX][3];
    
      --SET @V_DOCID =  INSERT INTO workflow.documento ( tpdid, esdid, docdsc )
      --VALUES ( @V_TPDID, @V_ESDID, 'Auditoria da SS' ) RETURNING docid;                

      
      IF( @V_AUDID )
      BEGIN
      	IF(NOT@V_AUD_DOCID)
	  	BEGIN
		  SET @V_DOCID =  INSERT INTO workflow.documento ( tpdid, esdid, docdsc )
                  VALUES ( @V_TPDID, @V_ESDID, 'Auditoria da SS' ) RETURNING docid;
                  
		  UPDATE fabrica.auditoria SET docid = @V_DOCID WHERE audid = @V_AUDID;
	  	END            
      END
      ELSE  
      BEGIN
	SET @V_DOCID =  INSERT INTO workflow.documento ( tpdid, esdid, docdsc )
        VALUES ( @V_TPDID, @V_ESDID, 'Auditoria da SS' ) RETURNING docid;
                  
        INSERT INTO fabrica.auditoria ( ansid, audrespfabrica, docid )
        VALUES ( @V_ANSID , 'ABDA GOUVEIA DE ALBUQUERQUE' , @V_DOCID ) ;
      END

            
      SET @IDX = @IDX + 1;
    END        
END

--ROLLBACK TRANSACTION;
COMMIT TRANSACTION;
