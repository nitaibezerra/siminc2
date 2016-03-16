<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/><?php 
	//Chamada de programa
	//include  APPRAIZ."includes/cabecalho.inc";
	echo monta_cabecalho_relatorio( '100' );
	$ctrid=$_GET['id'];
	 global $db;
	 $sql = "SELECT
			 ent.entnome as contratada,
			 tpc.tpcdsc || ' Nº ' ||  ctr.ctrnum || ' / ' || ctr.ctrano as numcontrato,
			 mod.moddsc as moddsc,
			 ctr.ctrobj as ctrobj,
			 case
			 when ctrvlrtotal is not null then ctrvlrtotal
			 else ctrvlrinicial
			 end as valor_total,
 			( SELECT SUM(a.total) FROM (
				 SELECT
				 		obs.valor AS total
 					FROM
 						contratos.ctcontrato c
					 JOIN entidade.entidade e ON e.entid = c.entidcontratada
					 JOIN contratos.hospital h ON h.hspid = c.hspid AND h.hspstatus = 'A'
					 JOIN contratos.hospitalug hu ON hu.hspid = h.hspid
					 JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid
					 JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
					 TRIM(es.ungcod) = TRIM(hu.ungcod) AND
					 TRIM(es.co_favorecido) = TRIM(e.entnumcpfcnpj)
					 JOIN contratos.ob_siafi obs ON TRIM(obs.empenho) = TRIM(es.nu_empenho) AND
					 TRIM(obs.unidade) = TRIM(hu.ungcod) AND
					 TRIM(obs.it_co_credor) = TRIM(e.entnumcpfcnpj)
 					WHERE
						 c.ctrid = {$ctrid} AND
						 obs.ob NOT IN (
						 SELECT
						 obf.obfnumero
						 FROM
						 contratos.faturacontrato fc
						 JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
						 WHERE
						 fc.ftcstatus = 'A' AND
						 fc.ctrid = {$ctrid}
						 )
 		UNION ALL
			 SELECT
			 SUM(obfvalor) AS total
			 FROM contratos.faturacontrato ftc
			 JOIN contratos.ordembancariafatura obf ON ftc.ftcid = obf.ftcid
			 WHERE ftc.ftcstatus = 'A'
			 AND ftc.ctrid = ctr.ctrid
			 ) AS a
			 ) AS valor_executado,
			 (select
			 sum(tdavlr) as total
			 from contratos.cttermoaditivo adi
			 where adi.tdastatus = 'A'
			 and adi.ctrid = ctr.ctrid) as valor_aditivo,
			 hspabrev || ' - '|| hspdsc as contratante
			 FROM contratos.ctcontrato ctr
			 left join contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
			 left join contratos.cttipocontrato tpc on ctr.tpcid = tpc.tpcid
			 left join entidade.entidade ent on ent.entid = ctr.entidcontratada
			 left join contratos.hospital h on h.hspid = ctr.hspid
			 WHERE ctr.ctrid = $ctrid";
 		$dados = $db->pegaLinha($sql);
 		
 		$sql = "SELECT
 		ct.ctrproccontr as protocolo,
 		ct.ctrobj as objeto,
 		
 		CASE WHEN ct.ctrvlrtotal IS NOT NULL THEN
 		ct.ctrvlrtotal
 		ELSE
 		(ct.ctrvlrinicial)+(SELECT
 		COALESCE(SUM(tdavlr),0) as total
 		FROM
 		contratos.cttermoaditivo adi
 		WHERE
 		adi.tdastatus = 'A' AND	adi.ctrid =ct.ctrid)
 		END AS valorcontrato,
 		nu_empenho,
 		ctrprcjurctr,
 		ctrprcjuradt,
 		ctrproccontr,
 		ctrjus,
 		ct.ctrdtassinatura,
 		ct.ctrdtiniciovig,
 		ct.ctrdtfimvig,
 		CASE
 		WHEN mun.muncod IS NULL THEN hspabrev || ' - ' || hspdsc
 		ELSE hspabrev || ' - ' || hspdsc || ' - ' || mun.mundescricao || '/' || mun.estuf
 		END AS hpsdsc
 		FROM
 		contratos.ctcontrato ct
 		NATURAL JOIN contratos.cttipocontrato tp
 		LEFT JOIN contratos.empenhovinculocontrato ec ON ct.ctrid=ec.ctrid
 		LEFT JOIN contratos.empenho_siafi es ON es.epsid =ec.epsid
 		LEFT JOIN contratos.hospital h ON ct.hspid=h.hspid
 		LEFT JOIN territorios.municipio mun ON mun.muncod = h.muncod
 		
 		 
 		WHERE ct.ctrid=$ctrid";

 		$dados = array_merge($dados,$db->pegaLinha($sql));

 		//dbg($dados, d);

 ?>
 <br /> <br />
     <table class="tabela" border="1 #f5f5f5" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class ="SubTituloDireita" align="right" style="width:20px;">Empenho: </td>
                <td  style="width:50px;"> 
                 <?
                 

                 echo trim($dados['nu_empenho']," ");
                 ?>
                </td>
                <td  class ="SubTituloDireita" align="right">Contratante:</td><td><?php echo $dados['contratante']?></td>
           
             </tr>
            <tr>
    			 <td  class ="SubTituloDireita" align="right">Contrato Nº:</td><td>
                <?php echo $dados['numcontrato'] ?></td>
                <td class ="SubTituloDireita" align="right">Processo:</td>
                <td style="width:80px;">
					<?php echo $dados['ctrproccontr']?>
                </td>
             </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Data Assinatura:</td>
                <td>
          		<?php echo formata_data($dados['ctrdtassinatura']) ?>	
                </td>
                <td class ="SubTituloDireita" align="right">Data publicação DOU:</td>
                <td>
          		<?php echo formata_data($dados['ctrdtdou']) ?>	
                </td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Objeto:</td>
                <td colspan=3>
          		<?php echo $dados['ctrobj'] ?>	
                </td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Fundamento Legal:</td>
                <td colspan=3>Lei nº 8.666/93 e Lei nº 8.078/90</td>
             </tr> 
             <tr>
                <td class ="SubTituloDireita" align="right">Modalidade:</td>
                <td colspan=3><?php echo $dados['moddsc'] ?>	</td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">Justificativa:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $dados['ctrjus'] ?>
                </td>
             </tr>
              <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">Nome da Contratada:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $dados['contratada'] ?>
                </td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right" style="width:20%">CNPJ:</td>
                <td style="width:80%" colspan=3>
          		<?php echo $entcnpjempresa ? mascara_global($entcnpjempresa, "##.###.###/####-##") : "" ?></td>
             </tr>
             <tr>
                <td class ="SubTituloDireita" align="right">Vigência do Contrato:</td>
                <td colspan=4>
          		<?php echo "De ".formata_data($dados['ctrdtiniciovig'])." à ".formata_data($dados['ctrdtfimvig']) ?>
                </td>
             </tr>

             <?php
             	$sql = "SELECT
							tda.tdanum,
							to_char(tda.tdadata::date,'YYYY-MM-dd') AS tdadata,
							tda.tdaobj,
							tda.tdavlr  as tdavlr,
							tda.tdainiciovig AS tdainiciovig,
							tda.tdafimvig AS tdafimvig,
							tda.tiaid
						FROM
							contratos.cttermoaditivo AS tda
						WHERE
						  tda.ctrid = $ctrid";
             	$rsDadosProcesso = $db->carregar( $sql );
             	if(is_array($rsDadosProcesso)){
					echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Aditivos:</td><td colspan=4>";
					$cont=1;
             		foreach($rsDadosProcesso as $aditivo){
						echo $cont."º aditivo - De ".formata_data($aditivo['tdainiciovig'])." à ".formata_data($aditivo['tdafimvig'])."<br>";
						$cont++;
					}
					echo "</td></tr>";
             	}
			?>
			 <tr>
                <td class ="SubTituloDireita" align="right">Valor Total:</td>
                <td>
          		<?php echo number_format($dados['valor_total'], 2, ',', '.') ?>
                </td>
                <td class ="SubTituloDireita" align="right">Conta Contábil:</td>
                <td>
          		<?php echo $dados['contacontabil'] ?>
                </td>
             </tr>
			<?php
             	$sql = "select
							'".(($desabilitado)?"":"<img src=\"../imagens/alterar.gif\" class=\"link\" onclick=\"editarFiscal(' || ent.entid || ')\" /> <img src=\"../imagens/excluir.gif\" class=\"link\" onclick=\"removerFiscal(' || fsc.fscid || ')\" />")."' as acao,
							entnome,
							entnumcpfcnpj
							from
								contratos.fiscalcontrato fsc
							inner join
								entidade.entidade ent ON ent.entid = fsc.entid
							where
								ctrid = $ctrid
							and
						fsc.fscstatus = 'A'";
             	$rsDadosProcesso = $db->carregar( $sql );
             	if(is_array($rsDadosProcesso)){
					echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Fiscais:</td><td colspan=4>";
             		foreach($rsDadosProcesso as $fiscais){
						echo $fiscais['entnome']." - ".$fiscais['entnumcpfcnpj']."<br>";

					}
					echo "</td></tr>";
             	}else
             		echo "<tr><td class =\"SubTituloDireita\" align=\"right\">Fiscais:</td><td colspan=4>Nenhum fiscal cadastrado.</td></tr>";

             ?>
              <tr>
                <td class ="SubTituloDireita" align="right" >Nº Pág. parecer jurídico</td>
                <td >Contrato:<?php echo $dados['ctrprcjurctr'] ?>
                <td colspan=2>Primeiro termo aditivo:<?php echo $dados['ctrprcjuradt'] ?></td>
			 </tr>

      </table>
      <br>
      <div align="center" class="notprint"><input type="button" name="btnImprimir" value="Imprimir" id="Imprimir" onclick="window.print();" align="center">
      <input type="button" name="fechar" value="Fechar" id="fechar" onclick="window.close();" align="center"></div>
      <?php echo "<script type=\"text/javascript\"> window.print();</script>"; ?>  
