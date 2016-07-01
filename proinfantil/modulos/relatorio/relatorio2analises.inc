<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
// Monta o título da página
monta_titulo( 'Relatório 2 Análises - Proinfância', '&nbsp;' );

$cabecalho =  array('Estado', 'Município','Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Creche', 'Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Integral / Pré-Escola',
                    'Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Creche', 'Qtd. Matrículas não computadas para recebimento do Fundeb – Tempo Parcial / Pré-Escola',
                    'Valor Repassado - Análise 1', 'Ano do Repasse - Análise 1', 'Valor Repassado - Análise 2','Ano do Repasse - Análise 2', 'Valor Total Repassado'
                    );


$sql = "
SELECT 			ede.estuf as estado
			, mun.mundescricao as municipio
			, vlr.mncintcre
			, vlr.mncintpre
			, vlr.mncparcre
			, vlr.mncparpre
                        , vlr.valorintcre + vlr.valorintpre + vlr.valorparcre +  vlr.valorparpre as analise1
			,(
									SELECT			
										
										'&nbsp;'||to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									--INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
                                                                                hst.aedid = '1034' 
                                                                        AND hst.docid = pin.docid
									ORDER BY		
										hst.htddata asc limit 1
									) as dataanalise11
			, vlr.valorintcre2 + vlr.valorintpre2 +  vlr.valorparcre2 + vlr.valorparpre2 as analise2
				 ,(
									SELECT			
										
										'&nbsp;'||to_char(hst.htddata,'YYYY') AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									WHERE 			
                                                                                hst.aedid =  '".AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO."'
                                                                        AND hst.docid = pin.docid
									ORDER BY		
										hst.htddata desc limit 1
									) as dataanalise21
				, vlr.valorintcre+vlr.valorintpre+vlr.valorparcre+vlr.valorparpre+
							   				vlr.valorintcre2+vlr.valorintpre2+vlr.valorparcre2+vlr.valorparpre2 as total_repasse
		
				 FROM
				obras2.obras obr 
			
			
			LEFT JOIN proinfantil.proinfantil  pin  ON obr.obrid = pin.obrid 
			INNER JOIN obras2.empreendimento e ON e.empid =  obr.empid
			INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'
			INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
			INNER JOIN workflow.documento d1 ON d1.docid = obr.docid --AND d1.tpdid = 48
			INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid 
			INNER JOIN entidade.endereco ede ON ede.endid = obr.endid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			INNER JOIN obras2.tipoobra AS tp ON obr.tobid = tp.tobid
			INNER JOIN entidade.entidade ent ON ent.entid = obr.entid
			
			LEFT  JOIN 
									(
									SELECT			
										pin.pinid, 
										to_char(hst.htddata,'YYYY')  AS anopagamento
									FROM 			
										workflow.historicodocumento hst
									INNER JOIN     proinfantil.proinfantil pin ON pin.docid = hst.docid
									WHERE 			
										hst.aedid = '".AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO."' 
									ORDER BY		
										hst.htddata DESC
									) as pag ON pag.pinid = pin.pinid
			
					LEFT JOIN entidade.funentassoc fea ON fea.entid = obr.entid 
					LEFT JOIN entidade.funcaoentidade fen ON fen.fueid = fea.fueid AND fen.funid=2 
					LEFT JOIN entidade.entidade pre ON pre.entid = fen.entid 
				
				
				INNER JOIN obras2.tipologiaobra AS tpl ON obr.tpoid = tpl.tpoid AND tpl.tpostatus = 'A' 
			LEFT  JOIN 
				(
				SELECT    		
					pin.pinid, 
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as mncintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as mncparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 3  AND val.vaatipo = 'I') THEN mat.alaquantidade ELSE 0 END) as totalintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 4  AND val.vaatipo = 'P') THEN mat.alaquantidade ELSE 0 END) as totalparpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse
						ELSE 0
					END) as valorintcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse 
						ELSE 0
					END) as valorintpre,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparcre,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse  
						ELSE 0
					END) as valorparpre,	    
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorintcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 1  AND val.vaatipo = 'I') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorintpre2,
					SUM(CASE WHEN (mat.timid = 3 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2 
						ELSE 0 
					END) as valorparcre2,
					SUM(CASE WHEN (mat.timid = 1 AND mat.titid = 2  AND val.vaatipo = 'P') 
						THEN ((mat.alaquantidade * val.vaavalor)/12)*pin.pinperiodorepasse2  
						ELSE 0
					END) as valorparpre2		    
				FROM
					proinfantil.mdsalunoatendidopbf  mat 
				INNER JOIN proinfantil.valoraluno 	val ON val.timid = mat.timid and vaastatus = 'A'
				INNER JOIN proinfantil.proinfantil 	pin ON pin.pinid = mat.pinid
				INNER JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
				INNER JOIN questionario.resposta 	res ON res.qrpid = que.qrpid
				WHERE
					res.perid = 1587 AND to_date(res.resdsc,'DD/MM/YYYY') BETWEEN vaadatainicial AND vaadatafinal
				GROUP BY
					pin.pinid
				) as 				vlr ON vlr.pinid = pin.pinid
			LEFT JOIN proinfantil.questionario 	que ON que.pinid = pin.pinid
			LEFT JOIN questionario.resposta 		res ON res.qrpid = que.qrpid AND res.perid = 1587
			
			 WHERE 	   
				obr.obrstatus = 'A' AND ent.entstatus = 'A' AND  obr.obrpercentultvistoria >= 90 AND  pf.prfid = 41  
				AND esd1.esdid IN (690, 693) AND obr.obridpai IS NULL AND tpl.tpoid in (16,9,10) 
							   and ((pinpareceraprovacao is not null OR pinanoseguinte is not null and pinperiodorepasse is not null) and 
			     ( pinpareceraprovacao2 IS NOT NULL AND pinperiodorepasse2 is not null))
	
                                                        group by estado,municipio, vlr.mncintcre ,pin.pinid
			, vlr.mncintpre
			, vlr.mncparcre
			, vlr.mncparpre
			, vlr.valorintcre
			, vlr.valorintcre2
			, vlr.valorintpre
			, vlr.valorintpre2
			, vlr.valorparcre
			, vlr.valorparcre2
			, vlr.valorparpre
			, vlr.valorparpre2 
ORDER BY   
			   	estado,municipio ";

              
$db->monta_lista($sql,$cabecalho,500000,5,'S','90%','N');

?>
