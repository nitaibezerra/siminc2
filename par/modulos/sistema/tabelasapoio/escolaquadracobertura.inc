<?php
global $db;

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

if(!in_array(PAR_PERFIL_SUPER_USUARIO,$perfil) && !in_array(PAR_PERFIL_COORDENADOR_GERAL,$perfil)) {
	print "<script>alert('Seu perfil não possui acesso a esta página!');</script>";
	print "<script>history.back();</script>";
	exit;	
}

if($_REQUEST['atualizar']){
	header('Content-Type: text/html; charset=iso-8859-1');
	$munSql = "SELECT		muncod AS codigo,
							mundescricao AS descricao
			   FROM 		territorios.municipio
			   WHERE 		estuf = '{$_REQUEST['uf']}'
			   ORDER BY 	mundescricao";	
	 $db->monta_combo( 'municipio', $munSql, 'S', 'Selecione', '', '', 'Selecione o item caso queira filtrar por Município!', '244','','municipio');
	exit();
}

if($_REQUEST['pesquisar']){
	header('Content-Type: text/html; charset=iso-8859-1');
	if($_REQUEST['ano']){
		$where[0] = "AND anocenso = '{$_REQUEST['ano']}'";
	}

	if($_REQUEST['codinep']){
		$where[1] = "AND codigoescola = '{$_REQUEST['codinep']}'";
	}

	if($_REQUEST['nome']){
        $nomeTmp = removeAcentos(str_replace("-"," ",utf8_decode($_REQUEST['nome'])));
		$where[2] = "AND UPPER(public.removeacento(nomeescola)) ilike '%{$nomeTmp}%'";
	}

	if($_REQUEST['uf']){
		$where[3] = "AND uf = '{$_REQUEST['uf']}'";
	}

	if($_REQUEST['municipio']){
		$where[4] = "AND muncod = '{$_REQUEST['municipio']}'";
	}

	$sql = "SELECT 	 	
						CASE WHEN COALESCE(obr.obras,0) = 0
							THEN '<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" onclick=\"excluir('||codigoescola||')\" style=\"cursor:pointer;\" />' 
							ELSE  '<img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\"Excluir\" />' END as acao, 
				 		esc.anocenso,
				 		esc.codigoescola,
				 		esc.nomeescola,
				 		esc.uf,
				 		esc.muncod,
				 		esc.nomemunicipio
  			FROM 		par.escolasadesaopac esc
			LEFT JOIN 	(SELECT count(entcodent) as obras, entcodent FROM obras.preobra WHERE prestatus = 'A' GROUP BY entcodent) obr ON trim(esc.codigoescola) = trim(obr.entcodent)	
			WHERE 		esc.status = 'A'
			$where[0] $where[1] $where[2] $where[3] $where[4]
	  		GROUP BY 	esc.anocenso,
				 		esc.codigoescola,
				 		esc.nomeescola,
				 		esc.uf,
				 		esc.muncod,
				 		esc.nomemunicipio,
				 		obr.obras
	  		ORDER BY 	uf,nomemunicipio";	
	
	$cabecalho = array('Ações', 'Ano Censo','Código Escola (INEP)','Nome Escola','UF','Código Município','Município');
	$db->monta_lista($sql,$cabecalho,100,20,'','','');
	exit();
}

if($_REQUEST['buscarEscolas']){
	header('Content-Type: text/html; charset=iso-8859-1');
	$where[0] = "";
	$where[1] = "";
	if($_REQUEST['codigoinep']){
		$where[0] = "AND cast(pk_cod_entidade as text) ilike '%{$_REQUEST['q']}%'";
	} 
	
	if($_REQUEST['nomeescola']){
		$where[1] = "AND no_entidade ilike '%{$_REQUEST['q']}%'";
	}	
	
	// Quando mudar o censo é necessário alterar o ano e o esquema educacenso_20XX
	$sql = "SELECT 			'2014' as anocenso, 
							pk_cod_entidade || ' - ' || no_entidade AS pk_cod_entidade, 
							no_entidade as no_escola, 
							muncod as co_municipio, 
							estuf as sg_uf, 
							mun.mundescricao
			FROM ".SCHEMAEDUCACENSO.".tab_entidade ent 
			INNER JOIN territorios.municipio mun ON fk_cod_municipio = cast(mun.muncod as numeric)
			WHERE 			1 = 1 $where[0] $where[1]
			LIMIT 100
			";
	
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	$q = strtolower($_REQUEST['q']);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if($_REQUEST['codigoinep']){
			echo "{$value['pk_cod_entidade']}|{$value['no_escola']}|{$value['anocenso']}|{$value['sg_uf']}|{$value['co_municipio']}|{$value['mundescricao']}\n";
		} else {
			echo "{$value['no_escola']}|{$value['pk_cod_entidade']}|{$value['anocenso']}|{$value['sg_uf']}|{$value['co_municipio']}|{$value['mundescricao']}\n";
		}
	}
	exit();
}

if ($_REQUEST['cadastrar']){
	$codigoinep = explode("-",$_REQUEST['codigoinep']);
	$codigoinep[0] = trim($codigoinep[0]);
	$sql_escola = "SELECT 			nomeescola 
				   FROM 			par.escolasadesaopac 
				   WHERE 			codigoescola = '{$codigoinep[0]}'";
	
	$rs_escola = $db->pegaUm($sql_escola);
	
	if(empty($rs_escola)){
		$sql_i = "INSERT INTO par.escolasadesaopac
				(anocenso, codigoescola, nomeescola, uf, muncod, nomemunicipio, usucpf, datainclusao)
	    		VALUES 
				('{$_REQUEST['anocenso']}','{$codigoinep[0]}','{$_REQUEST['nomeescola']}','{$_REQUEST['estuf']}','{$_REQUEST['muncod']}','{$_REQUEST['mundescricao']}','{$_SESSION['usucpf']}',now())";		
		$db->executar($sql_i);
		$db->commit();	
		echo 'S';
	} else {
		echo 'N';
	}
	exit();
}

if ($_REQUEST['excluir']){
	$sql_escola = "SELECT 		COUNT(obr.preid) 
				   FROM			par.escolasadesaopac esc
				   INNER JOIN 	obras.preobra obr ON trim(esc.codigoescola) = trim(obr.entcodent) 
				   WHERE 		trim(codigoescola) = '{$_REQUEST['codigo']}'";
	$rs_escola = $db->pegaUm($sql_escola);
	
	if(empty($rs_escola)){
		$sql_d = "UPDATE 	 	par.escolasadesaopac
				  SET			status = 'I'
				  WHERE			codigoescola = '{$_REQUEST['codigo']}'";		
		$db->executar($sql_d);
		$db->commit();	
		echo 'S';
	} else {
		echo 'N';
	}
	exit();	
}	

include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
monta_titulo($titulo_modulo,'');
?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<script type="text/javascript">
	$(function(){
		$("#codigoinep").autocomplete("par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A&buscarEscolas=true&codigoinep=true", {
			cacheLength:10,
			width: 440,		
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true	    		
		}).result(function(event, data, formatted) {
			$('#nomeescola').val(data[1]);
			$('#anocenso').val(data[2]);
			$('#estuf').val(data[3]);
			$('#muncod').val(data[4]);
			$('#mundescricao').val(data[5]);
		});

		$("#nomeescola").autocomplete("par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A&buscarEscolas=true&nomeescola=true", {
		  	cacheLength:10,
			width: 440,		
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true	    		
		}).result(function(event, data, formatted) {
			$('#codigoinep').val(data[1]);
			$('#anocenso').val(data[2]);
			$('#estuf').val(data[3]);
			$('#muncod').val(data[4]);
			$('#mundescricao').val(data[5]);
		});
	});

	function atualizarMunicipio(estado){
		$('#aguardando').show();
		$.ajax({
			type: "POST",
			url: 'par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A',
			data: { uf:estado,atualizar:true},
			async: true,
			success: function(data) {
				$("#listamunicipio").html(data);
			}	
		})
		$('#aguardando').hide();
	}
	
	function pesquisar(){
		$('#aguardando').show();
		$.ajax({
			type: "POST",
			url: 'par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A',
			data: { ano:$("#ano").val(),codinep:$("#codinep").val(),nome:$("#nome").val(),uf:$("#uf").val(),municipio:$("#municipio").val(),pesquisar:true},
			async: true,
			success: function(data) {
				$("#listaresultados").html(data);
			}	
		})
		$('#aguardando').hide();
	}	

	function cadastrar(){
	 	if(!$('#codigoinep').val()){
	 		alert('Informe o Código do INEP.');
	 		$('#codigoinep').focus()
	 		return false;
	 	}

	 	if(!$('#nomeescola').val()){
	 		alert('Informe o Nome da Escola.');
	 		$('#nomeescola').focus()
	 		return false;
	 	}

	 	if(!$('#anocenso').val()){
	 		alert('Informe o Ano Censo.');
	 		$('#anocenso').focus()
	 		return false;
	 	}

	 	if(!$('#estuf').val()){
	 		alert('Informe a UF.');
	 		$('#estuf').focus()
	 		return false;
	 	}

	 	if(!$('#muncod').val()){
	 		alert('Informe o Município.');
	 		$('#muncod').focus()
	 		return false;
	 	}		 			 			 	
	 	
	 	if(confirm("Confirma a inclusão desta escola no PAR?")) {
			$.ajax({
				type: "POST",
				url: 'par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A',
				data: {codigoinep:$("#codigoinep").val(),nomeescola:$("#nomeescola").val(),anocenso:$("#anocenso").val(),estuf:$("#estuf").val(),muncod:$("#muncod").val(),mundescricao:$("#mundescricao").val(),cadastrar:true},
				async: true,
				success: function(rs) {
					if(trim(rs) == 'N'){
						alert('Esta escola já está incluída no PAR.');
						$('#codigoinep').val('');
						$('#nomeescola').val('');
						$('#anocenso').val('');
						$('#estuf').val('');
						$('#muncod').val('');
						$('#mundescricao').val('');					
					} else {
						alert('Escola cadastrada com sucesso.');
						$('#codigoinep').val('');
						$('#nomeescola').val('');
						$('#anocenso').val('');
						$('#estuf').val('');
						$('#muncod').val('');
						$('#mundescricao').val('');							
					}	
				}	
			})	
		}
	}	

	function excluir(codigo){
		if(confirm("Deseja excluir esta escola?")) {
			$.ajax({
				type: "POST",
				url: 'par.php?modulo=sistema/tabelasapoio/escolaquadracobertura&acao=A',
				data: {codigo:codigo,excluir:true},
				async: true,
				dataType: "html",
				success: function(rs) {
					if(trim(rs) == "N"){
						alert('Esta escola não pode ser removida, pois existe uma obra vinculada.');
					} else {
						alert('Escola excluída com sucesso.');
						pesquisar();
					}	
				}	
			})	
		}
	}	
	
</script>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<form method="post" name="form" id="form" action="">
<input id="mundescricao" type="hidden" name="mundescricao" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Código INEP:</td>
		<td><?php echo campo_texto('codigoinep','S','','','50','150','','','', '', '', 'id="codigoinep"');?></td>
	</tr>			
	<tr>
		<td class="SubTituloDireita">Nome da Escola:</td>
		<td><?php echo campo_texto('nomeescola','N','','','50','100','','','', '', '', 'id="nomeescola"');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ano Censo:</td>
		<td>
			<?php
			$arrayAnoCenso = array(
				array('codigo' => '2011', 'descricao' => '2011'),
				array('codigo' => '2012', 'descricao' => '2012')
			);
			$db->monta_combo('anocenso', $arrayAnoCenso, 'N', 'Selecione', '', '', 'Selecione o item caso queira filtrar por Ano Censo!','244','','anocenso');?>
		</td>
	</tr>			
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
			<?php
			$ufSql = "SELECT 		estuf as codigo,
									estuf || ' - ' || estdescricao as descricao
			  		  FROM 			territorios.estado est
			          ORDER BY 		estdescricao";			
			$db->monta_combo('estuf', $ufSql, 'N', 'Selecione', 'atualizarMunicipio', '', 'Selecione o item caso queira filtrar por UF!','244','','estuf'); ?>
		</td>
	</tr>				
	<tr>
		<td class="SubTituloDireita">Município</td>
		<td>
			<?php 
			$munSql = "SELECT		muncod AS codigo,
									mundescricao AS descricao
			  		   FROM 		territorios.municipio
			   		   ORDER BY 	mundescricao";	
			$db->monta_combo('muncod', $munSql, 'N', 'Selecione', '', '', 'Selecione o item caso queira filtrar por Município!','244','','muncod'); ?>
		</td>
	</tr>					
    <tr>
      	<td colspan="2" class="SubTituloCentro">
      		<input type="button" class="botao" value="Cadastrar" onclick="cadastrar();">
      	</td>
    </tr>
</table>
</form>

<br>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
	<tr>
		<td class="subtitulodireita">Código INEP</td>
		<td><?php echo campo_texto('codinep','S','','','50','150','','','', '', '', 'id="codinep"');?></td>
	</tr>			
	<tr>
		<td class="subtitulodireita">Nome da Escola</td>
		<td><?php echo campo_texto('nome','S','','','50','100','','','', '', '', 'id="nome"');?></td>
	</tr>		
	<tr>
		<td class="subtitulodireita">UF</td>
		<td>
			<?php
			$ufSql = "SELECT 		estuf as codigo,
									estuf || ' - ' || estdescricao as descricao
			  		  FROM 			territorios.estado est
			          ORDER BY 		estdescricao";			
			$db->monta_combo('uf', $ufSql, 'S', 'Selecione', 'atualizarMunicipio', '', 'Selecione o item caso queira filtrar por UF!', '244','','uf'); ?>
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">Município</td>
		<td id="listamunicipio">
			<?php 
			$munSql = "SELECT		muncod AS codigo,
									mundescricao AS descricao
			  		  FROM 			territorios.municipio
			  		  WHERE 		estuf = ''
			   		  ORDER BY 		mundescricao";	
			$db->monta_combo('municipio', $munSql, 'S', 'Selecione', '', '', 'Selecione o item caso queira filtrar por Município!', '244','','municipio'); ?>
		</td>
	</tr>		
    <tr>
      	<td colspan="2" class="SubTituloCentro">
      		<input type="button" class="botao" value="Pesquisar" onclick="pesquisar();">
      	</td>
    </tr>
</table>

<div id="listaresultados">
<?php
$sql = "SELECT 	 	
					CASE WHEN COALESCE(obr.obras,0) = 0
						THEN '<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" onclick=\"excluir('||codigoescola||')\" style=\"cursor:pointer;\" />' 
						ELSE  '<img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\"Esta escola não pode ser removida, pois existe uma obra vinculada.\" />' END as acao, 
			 		esc.anocenso,
			 		esc.codigoescola,
			 		esc.nomeescola,
			 		esc.uf,
			 		esc.muncod,
			 		esc.nomemunicipio
  		FROM 		par.escolasadesaopac esc
		LEFT JOIN 	(SELECT count(entcodent) as obras, entcodent FROM obras.preobra WHERE prestatus = 'A' GROUP BY entcodent) obr ON trim(esc.codigoescola) = trim(obr.entcodent)	
		WHERE 		esc.status = 'A'
  		GROUP BY 	esc.anocenso,
			 		esc.codigoescola,
			 		esc.nomeescola,
			 		esc.uf,
			 		esc.muncod,
			 		esc.nomemunicipio,
			 		obr.obras
  		ORDER BY 	esc.uf, esc.nomemunicipio";

$cabecalho = array('Ações', 'Ano Censo','Código Escola (INEP)','Nome Escola','UF','Código Município','Município');
$db->monta_lista($sql,$cabecalho,100,20,'','','');
?>
</div>