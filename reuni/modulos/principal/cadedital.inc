<? ;
include APPRAIZ . 'reuni/www/funcoes.php';

print '<br/>';

$lan 		= $_REQUEST["lan"];
$lanauto 	= $lan."auto";
$ano 		=  substr($lan, 3, strlen($lan));
$lanid 		= $_REQUEST["lanid"];
$tipo 		= $_REQUEST["tipo"];
$monid 		= $_REQUEST["monid"];
$arqid 		= $_REQUEST["arqid"];
$anxid 		= $_REQUEST["anxid"];
$vagas_old  = $_REQUEST['vagas_old'];

switch($tipo) {		
	case 'E':	
		$texto = "da Portaria";
		$texto2 = "de Portarias";
		//total celula
		$tipoEP = "totalprovido_".$lanid; 
		//total(geral) do conjunto de celulas
		$totais_tipoEP = "totais_provido_"; 
	break;
	case 'P':	
		$texto = "do Edital";
		$texto2 = "de Editais";
		$tipoEP = "totalconcrusados_".$lanid; 
		$totais_tipoEP = "totais_concrusados_";  
	break;
	case 'A':	
		$texto = "da Portaria de Autorização";
		$texto2 = "de Portarias de Autorização";
		$tipoEP = "totalautorizado_".$lanid; 
		$totais_tipoEP = "totais_autorizado_"; 
	break;
}

$menu[0] = array("descricao" => "Lista ".$texto2, "link"=> "/reuni/reuni.php?modulo=principal/listareditais&acao=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."");
$menu[1] = array("descricao" => "Cadastro ".$texto2, "link"=> "/reuni/reuni.php?modulo=principal/cadedital&acao=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."");

echo montarAbasArray($menu, "/reuni/reuni.php?modulo=principal/cadedital&acao=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."");

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Cadastro ".$texto;
monta_titulo( $titulo, '' );

$sql_total_lancamentoauto = "SELECT l.".$lanauto."					
								 as lancamentoauto, 
								 l.nlcid 
							FROM reuni.lancamentocargo l			
							WHERE 
								 l.lanid = ".$lanid;

$total_lancamentoauto = $db->pegaLinha($sql_total_lancamentoauto);
$totais_tipoEP		= $totais_tipoEP.$total_lancamentoauto['nlcid'];

if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '')){
	switch($_REQUEST[evento]) {		
		case 'I':	
				$monid				 = $_REQUEST[monid];				
				$usucpf 			 = $_SESSION[usucpf];
				$montipo 			 = $tipo;
				$monnumeditalport  	 = $_REQUEST['monnumeditalport'];  
				$mondsc  			 = $_REQUEST['mondsc']; 
				$mondteditalport  	 = formata_data_sql($_REQUEST['mondteditalport']);
				$monnumdoueditalport = $_REQUEST['monnumdoueditalport'];  
				$mondtdoueditalport  = formata_data_sql($_REQUEST['mondtdoueditalport']);
				$monnumvagas 		 = $_REQUEST['monnumvagas']; 
				
				$arquivo 			 = $_FILES['arquivo'];
				$tpaid				 = $_REQUEST['tpaid'];
				$arqdescricao 		 = $_REQUEST['arqdescricao'];
				
				$dados_arq = array('arqdescricao' => $arqdescricao, 'tpaid' => $tpaid, 'lanid' => $lanid);				
			
			if($monid){	
				
				$dados_arq[monid] = $monid;
				
				$sql = "
				SELECT 
					d.monid				
				FROM  reuni.dadosmonitoramento d
				WHERE 
				d.monid = ".$monid."
				";					
				$dados = $db->pegaUm($sql);	
			}
			
			// caso não exista o edital/portaria insere um novo
			if(!$dados){
				$sql_I = "INSERT 
						  INTO reuni.dadosmonitoramento
							  (usucpf, lanid, montipo, monnumeditalport, mondsc, mondteditalport, mondtdoueditalport, monnumvagas, monano, monnumdoueditalport, mondtinclusao)
						  VALUES 
						  	   ('".$usucpf."', '".$lanid."', '".$montipo."', '".$monnumeditalport."', '".$mondsc."', '".$mondteditalport."', '".$mondtdoueditalport."', ".$monnumvagas.", '".$ano."', '".$monnumdoueditalport."', now())
						  RETURNING	monid
						  ";	
				$monid = $db->pegaUm($sql_I);
				$dados_arq[monid] = $monid;
				
				if(isset($arquivo['name']) && $arquivo['name'] != ''){
					EnviarArquivo($arquivo, $dados_arq);
				}
				
				$saldovagas = $monnumvagas - $vagas_old ;				
				echo "<script>
						window.opener.document.getElementsByName('".$tipoEP."')[0].value = Number(window.opener.document.getElementsByName('".$tipoEP."')[0].value) + $saldovagas;
						window.opener.document.getElementById('".$totais_tipoEP."').innerHTML = Number(window.opener.document.getElementById('".$totais_tipoEP."').innerHTML) + $saldovagas;
					  </script>";	
			//caso exista o indicador altera os dados do mesmo	
			}else{				
				$sql_U = "
					UPDATE reuni.dadosmonitoramento
					SET
					 monnumeditalport 	 = '".$monnumeditalport."',
					 mondsc			  	 = '".$mondsc."',					
					 mondteditalport  	 = '".$mondteditalport."',	
					 monnumdoueditalport = '".$monnumdoueditalport."',
					 mondtdoueditalport	 = '".$mondtdoueditalport."',					
					 monnumvagas  		 = ".$monnumvagas."	
					WHERE monid = ".$monid."	
				";	
				$db->executar($sql_U);
				if(isset($arquivo['name']) && $arquivo['name'] != ''){
					EnviarArquivo($arquivo, $dados_arq);
				}
				
				$saldovagas = $monnumvagas - $vagas_old ;
				
				echo "<script>
						window.opener.document.getElementsByName('".$tipoEP."')[0].value = Number(window.opener.document.getElementsByName('".$tipoEP."')[0].value) + $saldovagas;
						window.opener.document.getElementById('".$totais_tipoEP."').innerHTML = Number(window.opener.document.getElementById('".$totais_tipoEP."').innerHTML) + $saldovagas;
					  </script>";	
			}
			
			$db->commit();
					
			echo "<script>
					alert('Dados salvos com sucesso.');					
					window.location = '?modulo=principal/cadedital&acao=A&evento=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."&monid=".$monid."';
				  </script>";
			exit;
		break;
		
		case 'A': 
			$lanid = $_REQUEST['lanid']; 	
			$monid = $_REQUEST['monid']; 			
			$sql = "
				SELECT 					
					d.usucpf, 
					d.lanid, 
					d.montipo, 
					d.monnumeditalport, 
					d.mondsc, 
					d.mondteditalport, 
					d.mondtdoueditalport, 
					d.monnumvagas, 
					d.monano, 
					d.monnumdoueditalport, 
					d.mondtinclusao		
				FROM  reuni.dadosmonitoramento d
				WHERE 
				d.monid = ".$monid."
				";
			$dados = $db->pegaLinha($sql);	
			
			$usucpf 			 = $dados['usucpf'];
			$lanid 				 = $dados['lanid'];
			$monnumeditalport  	 = $dados['monnumeditalport'];  
			$mondsc  			 = $dados['mondsc']; 
			$mondteditalport  	 = formata_data($dados['mondteditalport']);
			$monnumdoueditalport = $dados['monnumdoueditalport'];  
			$mondtdoueditalport  = formata_data($dados['mondtdoueditalport']);
			$monnumvagas 		 = $dados['monnumvagas'];
			$vagas_old 			 = $monnumvagas;
		break;
	}	
}
if(isset($_REQUEST[evento_arquivo]) && ($_REQUEST[evento_arquivo] != '')){
	
	switch($_REQUEST[evento_arquivo]){
		case 'EA':
			$dados_arq = array('arqid' => $arqid, 'anxid' => $anxid);
			DeletarDocumento($dados_arq);
			echo "<script>
					alert('Dados salvos com sucesso.');					
					window.location = '?modulo=principal/cadedital&acao=A&evento=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."&monid=".$monid."';
				  </script>";
			exit;
		break;
		case 'DA':
			$dados_arq = array('arqid' => $arqid);
			DownloadArquivo($dados_arq);
			exit;
		break;
		case 'I':
			exit;
		break;
	}
}
//obtendo todais
$sql = "SELECT 
			count(d.monid) as qtd
			FROM  reuni.dadosmonitoramento d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = 'E' AND
			d.monstatus = 'A'
			";
	
$total_providos = $db->pegaUm($sql);

$sql = "SELECT 
			count(d.monid) as qtd
			FROM  reuni.dadosmonitoramento	d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = 'P' AND
			d.monstatus = 'A'
			";
	
$total_concursados = $db->pegaUm($sql);

$habil = habilitado($total_lancamentoauto['lancamentoauto'], ($total_providos + $total_concursados ));
$habil = $habil ? 'S' : 'N'; 
function habilitado($autorizados, $monitorados){	
	return true;
	if( $autorizados > $monitorados ){
		return true;
	}
}

?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
</head>
<body>
<form method="POST"  name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="evento" id="evento" value="I">	
	<input type="hidden" name="evento_arquivo" id="evento_arquivo" value="I"/>
	<input type="hidden" name="tipo" id="tipo" value="<?=$tipo?>">
	<input type="hidden" name="lan" id="lan" value="<?=$lan?>">
	<input type="hidden" name="lanid" id="lanid" value="<?=$lanid?>">
	<input type="hidden" name="arqid" id="arqid" value="<?=$arqid?>">
	<input type="hidden" name="anxid" id="anxid" value="<?=$arqid?>">
	<input type="hidden" name="monid" id="monid" value="<?=$monid?>">
	<input type="hidden" name="vagas_old" id="vagas_old" value="<?=$vagas_old?>">

	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' width="30%" class="SubTituloDireita">Número <?=$texto?>:</td>
	    <td><?=campo_texto('monnumeditalport','S',$habil,'',15,10,'','');?></td>
	</tr>
	<tr>
	    <td align='right' class="SubTituloDireita">Descrição:</td>
	    <td><?=campo_textarea( 'mondsc','S', $habil, '', 60, 2, 300 );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Data <?=$texto?>:</td>
	    <td><?=campo_data('mondteditalport','S',$habil,'','','','');?></td>
	</tr>
	<tr>
	    <td align='right' class="SubTituloDireita">Número do DOU:</td>
	     <td><?=campo_texto('monnumdoueditalport','S',$habil,'',15,10,'','');?></td>
	</tr> 
	<tr>
		<td align='right' class="SubTituloDireita">Data do DOU:</td>
	    <td><?=campo_data('mondtdoueditalport','S',$habil,'','','','');?></td>
	</tr>
	<tr>
	    <td align='right' class="SubTituloDireita">Número de vagas <?=$texto?>:</td>
	     <td><?=campo_texto('monnumvagas','S',$habil,'',15,'','###########','');?></td>
	</tr> 
	<? 
	if($habil == 'S'){
	?>
	<!--  
	<tr bgcolor="#cccccc">
	      <td></td>
	      <td>&nbsp;
	    
	    <input type="button" class="botao" name="btassociar" value="Novo Edital/Portaria" onclick="novo();">	  	  
	    <input type="button" class="botao" name="btassociar" value="Limpar" onclick="limpar();">
	    <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('I', '<?=$tipoEP?>', '<?=$lanid?>', '<?=$totais_autorizado?>', '<?=$totais_tipoEP?>');">	  	  	
	   </td>	  	  
	</tr> -->
	<tr bgcolor="#cccccc">
	      <td colspan="2" align="center"><b>Anexos</b>
	   </td>	  	  
	</tr>
	
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<input type="file" name="arquivo"/>							
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%">Tipo:</td>
			<td><?php			
			$sql = "
				SELECT tpaid AS codigo, tpadsc AS descricao 
					FROM reuni.tipoarquivo
			";
			
			$db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100');
		?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 100 );?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<!--<input type="button" class="botao" name="btassociar" value="Novo Anexo" onclick="novoAqr();">	  	  
			    <input type="button" class="botao" name="btassociar" value="Limpar" onclick="limparArq();">
			    --><input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeterArq('I', '<?=$lanid?>', '<?=$arqid?>');">
			</td>
		</tr>
	</table>
	<?
	}
	?> 
</form>
<?
if($monid){ 

	$sql = "SELECT
			'<center><a href=\"#\" onclick=\"javascript:excluirDocumento(\'EA\',' ||aqb.lanid|| ',' || arq.arqid || ',' || aqb.anxid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,						
			to_char(aqb.anxdtinclusao,'DD/MM/YYYY'),
			tarq.tpadsc,
			'<a style=\"cursor: pointer; color: blue;\" onclick=\"javascript:downloadArquivo(\'DA\',' ||aqb.lanid|| ',' || arq.arqid || ');\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
			arq.arqtamanho || ' ' as tamanho ,
			arq.arqnome,								
			usu.usunome
		FROM
			public.arquivo arq 
			INNER JOIN reuni.anexos aqb ON aqb.arqid = arq.arqid
			INNER JOIN reuni.tipoarquivo tarq ON tarq.tpaid = aqb.tpaid 
			INNER JOIN seguranca.usuario usu ON usu.usucpf = aqb.usucpf
			
		WHERE
			aqb.monid = ".$monid." AND
			aqb.anxstatus = 'A'
		";
	$cabecalho = array( "Ação", 
						"Data Inclusão",
						"Tipo Arquivo",
						"Nome Arquivo",
						"Tamanho (kbs)",
						"Descrição Arquivo",
						"Responsável");
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
}
?>

<script type="text/javascript">

function submeter(evento, campo, lanid, totais_autorizado, totais_tipoEP){	
	if(validar()){		
		window.opener.document.getElementsByName(campo)[0].value = Number(window.opener.document.getElementsByName(campo)[0].value) + 1;
		window.opener.document.getElementById(totais_tipoEP).innerHTML = Number(window.opener.document.getElementById(totais_tipoEP).innerHTML) + 1;		
		document.getElementById('evento').value = evento;
		document.getElementById('lanid').value = lanid;
		document.formulario.submit();	
	}
}

function submeterArq(evento, lanid, arqid){		
	if(validar()){
		document.getElementById('evento').value = evento;
		document.getElementById('lanid').value = lanid;
		document.getElementById('arqid').value = arqid;

		/** verificando se foi adicionado arquivo e fazendo as criticas caso afirmativo*/
		if((document.formulario.arquivo.value != "") || (document.formulario.tpaid.value != "") || (document.formulario.arqdescricao.value != "")){
			if(validarArquivo()){
				document.formulario.submit();	
			}
		}else{
			document.getElementById('evento_arquivo').value = '';
			document.formulario.submit();	
		}
	}	
}

function novo(){
	document.getElementById('evento').value = '';
	document.getElementById('monid').value = '';	
	document.formulario.submit();		
}

function novoArq(){
	document.getElementById('evento').value = '';
	document.getElementById('arqid').value = '';	
	document.formulario.submit();		
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.monnumeditalport.value = '';
	document.formulario.mondsc.value = '';
	document.formulario.mondteditalport.value = '';
	document.formulario.monnumdoueditalport.value = '';
	document.formulario.mondtdoueditalport.value = '';
	document.formulario.monnumvagas.value = '';
}

function limparArq(){	
	document.getElementById('evento').value = '';	
	document.formulario.arquivo.value = '';
	document.formulario.tpaid.value = '';
	document.formulario.arqdescricao.value = '';
}

function validar(){	
	var msg = "";
	if(document.formulario.monnumeditalport.value == ''){
		msg+="O preenchimento do campo \"Número\" é obrigatório.\n";
	}
	if(document.formulario.mondsc.value == ''){
		msg+="O preenchimento do campo \"Descrição\" é obrigatório.\n";
	}
	if( ! validaData(document.formulario.mondteditalport)){
		msg+="O campo \"Data\" não foi informado ou foi informado incorretamente.\n";
	}
	if(document.formulario.monnumdoueditalport.value == ''){
		msg+="O preenchimento do campo \"Número do DOU \" é obrigatório.\n";
	}
	if( ! validaData(document.formulario.mondtdoueditalport)){
		msg+="O campo \"Data do DOU\" não foi informado ou foi informado incorretamente.\n";
	}
	if(document.formulario.monnumvagas.value == ''){
		msg+="O preenchimento do campo \"Número de vagas\" é obrigatório.\n";
	}
	if(msg != ""){
		alert(msg);
		return false;
	}else return true;	
}

function validarArquivo(){
	
	if(document.formulario.arquivo.value == ""){
		alert("Campo Arquivo obrigatório.");
		document.formulario.arquivo.focus();
		return false;
	}
	if(document.formulario.tpaid.value == ""){
		alert("Campo Tipo obrigatório.");
		document.formulario.tpaid.focus();
		return false;
	}
	if(document.formulario.arqdescricao.value == ""){
		alert("Campo Descrição obrigatório.");
		document.formulario.arqdescricao.focus();
		return false;
	}	
	return true;
}

function excluirDocumento(evento, lanid, arqid, anxid){
	if(confirm("Deseja realmente excluir este documento ?")){
		document.getElementById('evento').value = '';
		document.getElementById('evento_arquivo').value = evento;
		document.getElementById('lanid').value = lanid;
		document.getElementById('arqid').value = arqid;
		document.getElementById('anxid').value = anxid;
		document.formulario.submit();		
	}
}
function downloadArquivo (evento, lanid, arqid){
	document.getElementById('evento').value = '';
	document.getElementById('evento_arquivo').value = evento;
	document.getElementById('lanid').value = lanid;
	document.getElementById('arqid').value = arqid;
	document.formulario.submit();		
}
</script>
</body>
</html>