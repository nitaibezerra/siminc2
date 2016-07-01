<?php
/* $_GET
 * Trata string de subações transfornando-as para sql
 */
$sbaidList = explode(';', $_GET['sbaid']);
array_pop($sbaidList);
$sbaidList = (string) implode(', ', $sbaidList);
$modoSub = $_REQUEST['tipo'];

/**** TIPO DE PARECER ****
 * $_GET['param'] == 1 => Físico/Finânceiro
 * $_GET['param'] == 2 => Engenharia
 **** AÇÃO ****
 * carregar dados do formulário e sql;
 * inserir/atualizar pareceres "físico/financeiro" e "engenharia";
 */
if ($_GET['param'] == 1){
	#### DADOS identificadores "físico/financeiro ####
	$campo = "parfisicofinanceiro";
	$label = "Físico/Finânceiro";
	
	if ($_POST['parfisicofinanceiro']):
	
		##### ATUALIZAR parecer físico/financeiro #####
		if ($_POST['idFinanceiro']):
			$sql = sprintf("UPDATE 
								cte.parecerpar 
							SET 
								partexto = '%s'
							WHERE 
								parid = %d",
							$_POST['parfisicofinanceiro'],
							$_POST['idFinanceiro']);
			$db->executar($sql);
			unset($sql);
			foreach($modoSub as $indice=>$valor ){
			$sql = sprintf("UPDATE 
								cte.subacaoindicador 
							SET 
								partipoconv = %d
							WHERE 
								sbaid = %d",
							$valor,
							$indice);
			$db->executar($sql);
			unset($sql);
			}
			
		##### INSERIR parecer físico/financeiro #####	
		else:
			$sql = sprintf("INSERT INTO cte.parecerpar
								(usucpf, tppid, pardata, partexto)
							VALUES
								('%s', %d, now(), '%s') RETURNING parid",
							$_SESSION['usucpf'],
							1,
							$_POST['parfisicofinanceiro']
							);
			$parid = $db->pegaUm($sql);
			unset($sql);
			foreach($modoSub as $indice=>$valor ){
				$sql = sprintf("UPDATE 
									cte.subacaoindicador 
								SET 
									partipoconv = %d
								WHERE 
									sbaid = %d",
								$valor,
								$indice);
				$db->executar($sql);
				unset($sql);
			}
			
			##### VINCULAR parecer à subação convênio #####
			$sql = sprintf("INSERT INTO cte.subacaoconvenioparecerpar 
								(parid, sbcid)
								(SELECT %d, sbcid FROM cte.subacaoconvenio WHERE sbaid in (%s))",
							$parid,
							$sbaidList);
			$db->executar($sql);
			unset($sql, $parid);	
		endif;
		$db->commit();
		die('<script>
				alert("Operação realizada com sucesso!");
				location.href="?modulo=principal/popupListSubConv&acao=A";
			 </script>');		
	endif;
}else{
	#### DADOS identificadores "engenharia" ####
	$campo = "parengenharia";
	$label = "Engenharia";
	
	if ($_POST['parengenharia']):
	
		##### ATUALIZAR parecer engenharia #####	
		if ($_POST['idEngenharia']):
			$sql = sprintf("UPDATE cte.parecerpar SET partexto = '%s' WHERE parid = %d",
							$_POST['parengenharia'],
							$_POST['idEngenharia']);
			$db->executar($sql);	
			
		##### INSERIR parecer engenharia #####				
		else:
			$sql = sprintf("INSERT INTO cte.parecerpar
								(usucpf, tppid, pardata, partexto)
							VALUES
								('%s', %d, now(), '%s') RETURNING parid",
							$_SESSION['usucpf'],
							2,
							$_POST['parengenharia']);
			$parid = $db->pegaUm($sql);
			unset($sql);

			##### VINCULAR parecer à subação convênio #####			
			$sql = sprintf("INSERT INTO cte.subacaoconvenioparecerpar 
								(parid, sbcid)
								(SELECT %d, sbcid FROM cte.subacaoconvenioparecerpar WHERE parid = %d)",
							$parid,
							$_GET['parecer']);
			$db->executar($sql);		
		endif;
		$db->commit();
		die('<script>
				alert("Operação realizada com sucesso!");
				location.href="?modulo=principal/popupListSubConv&acao=A";
			 </script>');		
	endif;
}

########### Carregar #############
// Dados para atualização
// Complemento do select
if (!empty($_GET['parecer'])){
	$sql = sprintf("SELECT
					 DISTINCT
					 pp.partexto AS parfisicofinanceiro,
					 pp.parid AS idfinanceiro,
					 pp.partipoconv AS tipoconv,
					 eng.partexto AS parengenharia,
					 eng.parid AS idengenharia
					FROM
					 cte.parecerpar pp
					 INNER JOIN cte.subacaoconvenioparecerpar scp ON pp.parid = scp.parid
					 left JOIN (SELECT
								 pp1.parid,
								 pp1.partexto,
								 scp1.sbcid
							    FROM
								 cte.parecerpar pp1
								 INNER JOIN cte.subacaoconvenioparecerpar scp1 ON pp1.parid = scp1.parid
							    WHERE
								 tppid = 2) eng ON eng.sbcid = scp.sbcid 
					WHERE
					 pp.parid = %d", 
					$_GET['parecer']);
	$dados = (array) $db->carregar($sql);

	extract($dados[0], EXTR_OVERWRITE);

	$from  = "INNER JOIN cte.subacaoconvenioparecerpar sacp ON sacp.sbcid = sac.sbcid";
	$where = "sacp.parid = ".$_GET['parecer'];
}else{
	$where = "sa.sbaid in ({$sbaidList})";
}

$sql = sprintf("SELECT
				 CASE 
				  WHEN ins.muncod is null THEN est.estuf || ' - ' || est.estdescricao  
				  ELSE mun.estuf || ' - ' || mun.mundescricao
				 END as title 
				FROM
				 cte.instrumentounidade ins
				 LEFT JOIN territorios.estado est ON est.estuf = ins.estuf
				 LEFT JOIN territorios.municipio mun ON mun.muncod = ins.muncod  
				WHERE
				 inuid = %d",$_SESSION['inuid']);
$federacao = $db->pegaUm($sql);
?>
<html>
  <head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
  </head>	
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Cadastro de Parecer', '<img src=\'/imagens/obrig.gif\'> Campo obrigatório'  );
?>
<form action="" method="post" name="form" onsubmit="javascript:return valida();">
<table border="0" width="95%" cellspacing="0" cellpadding="2" align="center">
	<TR>
		<td colspan="2" style="padding: 0pt 15px; background-color: rgb(250, 250, 250); color: rgb(64, 64, 64); border: 1px solid #dfdfdf;">
			<div style="float: left; position: relative;">
				<h3 title="Unidade da Federação">
					<?= $federacao; ?>
				</h3>
			</div>
		</td>
	</TR>
	<TR>
        <td align='right' class="SubTituloDireita">Tipo parecer:</td>
		<TD>
			<b><?=$label ?></b>
		</TD>
	</TR>
	<TR>
        <td align='right' class="SubTituloDireita">Parecer:</td>
		<TD>
		<?=campo_textarea($campo, 'S', 'S', 'Descrição do arquivo', '80', '5', '250'); ?>
		</TD>
	</TR>
	<?php
	if ($_GET['param'] == 1){
		$sqlRadio = "SELECT 
				'1' AS codigo,
				'Novo Convênio' AS descricao
			 UNION ALL
			 SELECT
			 	'2' AS codigo,
			 	'Aditivo' AS descricao";
	?>
	<!-- 
		<TR>
			<td align='right' class="SubTituloDireita">Tipo:</td>
			<TD>
				<?=$db->monta_radio('tipoconv',$sqlRadio,'S','')?>
			</TD>
		</TR>
		-->
	<?php			
	}
	?>
<!-- 
	<TR>
        <td align='right' class="SubTituloDireita">Tipo parecer:</td>
		<TD>
			<b>Engenharia</b>
		</TD>
	</TR>
	<TR>
        <td align='right' class="SubTituloDireita">Parecer:</td>
		<TD>
		<?=campo_textarea('parengenharia', 'N', 'S', 'Descrição do arquivo', '80', '5', '250'); ?>
		</TD>
	</TR>	
-->	
<TR>
<TD colspan="2">	
<?php


$sql = sprintf("SELECT
				 DISTINCT
				 '<input name=\"tipo[' || sa.sbaid || ']\" type=\"radio\" value=\"1 \" />' as novo,
				  '<input name=\"tipo[' || sa.sbaid || ']\" type=\"radio\" value=\"2 \" />' as aditivo,
				 sbadsc		 
				FROM
				 cte.subacaoindicador sa
				 INNER JOIN cte.subacaoconvenio sac ON sac.sbaid = sa.sbaid
				 %s 
				WHERE
				 %s
				 ",
				$from,
				$where);

$cabecalho = array('Novo Convênio','Aditivo ','<b>Subações que será dado o parecer</b>');
$db->monta_lista_simples($sql,$cabecalho, 1000, 1000, 'N', '95%');
?>
</TD>
</TR>
<TR bgcolor="#C0C0C0">
     	<td colspan="2" align="center">
            <input type="submit" class="botao" name="submit" value="Salvar Parecer" />
            <input type="button" class="botao" name="back" value="Voltar" onclick="location.href='?modulo=principal/popupListSubConv&acao=A';" />
            <input type="hidden" name="idFinanceiro" value="<?=$idfinanceiro ?>" />
            <input type="hidden" name="idEngenharia" value="<?=$idengenharia ?>" />
        </td>
    </tr>        
</table>
</form>
</body>
	<script type="text/javascript">
	/*
	* Function, Validar campos obrigatório
	*/
	function valida(){
		d = document;
		element1 = d.getElementById('<?=$campo ?>');
		
		if (element1.value == ''){
			element1.focus();
			element1.select();
			alert('O campo parecer é obrigatório!');
			return false;
		}
		/*
			if(d.form.elements["tipo[1447107]"].check) {
	    		if(document.form.elements["tipo[1447107]"].value == '0' && document.form.elements["tipo[]"].value == '0') {
	    			alert('O campo tipo e obrigatório para cada Subação.');
	    			return false;
	    		}
	    	}
			if(d.form.elements["tipo[]"].length) {
			alert("teste2");
				
    			for(i = 0; i < document.form.elements["tipo[]"].length; i++) {
    				if(document.form.elements["tipo[]"][i].value == '0' && document.form.elements["tipo[]"][i].value == '0') {
    					alert('O campo tipo e obrigatório para cada Subação.');
    					return false;
    				}
    			}
    		}
    		*/
		<?php
		if ($_GET['param'] == 1){
		?>
			element2 = d.getElementsByName('tipoconv');
			if (element2[0].checked == false && element2[1].checked == false){
				alert('O campo tipo deve ser preenchido!');
				element2[0].focus();
				return false;
			}
			
		<?php
		}
		?>
		return true;
	}
	</script>

