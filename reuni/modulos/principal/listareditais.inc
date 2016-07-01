<?
// monta cabeçalho
include APPRAIZ . 'reuni/www/funcoes.php';
$db->cria_aba( $abacod_tela, $url, '' );
?>
<br>

<? 

$lan 	 = $_REQUEST[lan];
$lanauto = $lan."auto";
$ano 	 =  substr($lan, 3, strlen($lan));
$lanid   = $_REQUEST[lanid];
$tipo  	 = $_REQUEST[tipo];

switch($tipo) {		
	case 'E':	
		$titulo = "Portaria";		
		$titulo2 = " da Portaria";
		//total celula
		$tipoEP = "totalprovido_".$lanid; 
		//total(geral) do conjunto de celulas
		$totais_tipoEP = "totais_provido_";
	break;
	case 'P':	
		$titulo = "Edital";
		$titulo2 = " do Edital";
		$tipoEP = "totalconcrusados_".$lanid; 
		$totais_tipoEP = "totais_concrusados_";  
	break;
	case 'A':	
		$titulo = "Portaria de Autorização";
		$titulo2 = " da Portaria de Autorização";
		$tipoEP = "totalautorizado_".$lanid; 
		$totais_tipoEP = "totais_autorizado_"; 
	break;
}
monta_titulo( $titulo, '' );

//dados lançamentos
$sql = "SELECT 				
				cu.caudesc as campus,
				ca.cardesc as cargo,
				nc.nlcdesc as nivel		
			FROM 
				reuni.lancamentocargo 	AS lc						
			INNER JOIN reuni.campusuniversitario AS cu ON (cu.cauid  = lc.cauid)
			INNER JOIN reuni.campuslancamento    AS cl ON (cl.calid  = cast(lc.calid as integer))
			LEFT  JOIN reuni.cargo		         AS ca ON (ca.carcod = cl.carcod)
			INNER JOIN reuni.nivellimitecargo    AS nc ON (nc.nlcid  = lc.nlcid)
			
			WHERE				
				lc.lanid = ".$lanid."";
$dados = $db->pegaLinha($sql);

//obtendo todais
$sql = "SELECT 
			sum(d.monnumvagas) as qtd
			FROM  reuni.dadosmonitoramento d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = 'E' AND
			d.monstatus = 'A'
			";
	
$total_providos = $db->pegaUm($sql);

$sql = "SELECT 
			sum(d.monnumvagas) as qtd
			FROM  reuni.dadosmonitoramento	d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = 'P' AND
			d.monstatus = 'A'
			";
	
$total_concursados = $db->pegaUm($sql);

$sql = "SELECT 
			sum(d.monnumvagas) as qtd
			FROM  reuni.dadosmonitoramento	d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = 'A' AND
			d.monstatus = 'A'
			";
	
$total_autorizado = $db->pegaUm($sql);

$sql_total_lancamentoauto = "SELECT l.".$lanauto."					
								 as lancamentoauto, 
								 l.nlcid 
							FROM reuni.lancamentocargo l			
							WHERE 
								 l.lanid = ".$lanid."";
$total_lancamentoauto = $db->pegaLinha($sql_total_lancamentoauto);
$totais_tipoEP		  = $totais_tipoEP.$total_lancamentoauto['nlcid'];

$habil = habilitado($total_lancamentoauto['lancamentoauto'], ($total_providos + $total_concursados ));
$habil = $habil ? 'S' : 'N'; 
function habilitado($autorizados, $monitorados){	
	return true;
	if( $autorizados > $monitorados ){
		return true;
	}
}

if(isset($_REQUEST[evento]) && ($_REQUEST[evento] != '') ){
	$lanid = $_REQUEST['lanid'];
	switch($_REQUEST[evento]){		
	
		case 'E':	
			$monid = $_REQUEST[monid]; 			
			$sql = "
				SELECT 
					d.monnumvagas					
				FROM  reuni.dadosmonitoramento d
				WHERE 
				d.monid = ".$monid."
				";
			$vagas = $db->pegaUm($sql);
			
			$sql_D = "
					UPDATE reuni.dadosmonitoramento SET monstatus = 'I' where monid = ".$monid."";	
			$db->executar($sql_D);
			$db->commit();			
			echo "<script>
					window.opener.document.getElementsByName('".$tipoEP."')[0].value = Number(window.opener.document.getElementsByName('".$tipoEP."')[0].value) - $vagas;
					window.opener.document.getElementById('".$totais_tipoEP."').innerHTML = Number(window.opener.document.getElementById('".$totais_tipoEP."').innerHTML) - $vagas;
					alert('Registro removido com sucesso.');
					window.location = '?modulo=principal/listareditais&acao=A&evento=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."';
				  </script>";
		exit;
		break;			
	}
}

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario">
<input type="hidden" name="evento" id="evento" value="I">
<input type="hidden" name="monid" id="monid" value="<?=$monid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
	<td  align='right' width="25%" class="SubTituloDireita">Exercício:</td>
    <td><?=$ano;?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Nível:</td>
    <td><?=$dados['nivel']?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Campus/Cargo:</td>
    <td><?
    		if($dados['cargo']){
    			echo($dados['campus']." - ".$dados['cargo']);
    		}else{
    			echo($dados['campus']);
    		} 
    	?>
    </td>
</tr>
<!--<tr>
	<td  align='right' class="SubTituloDireita">Autorizado:</td>
    <td><?=$total_lancamentoauto['lancamentoauto']?></td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Total de vagas:</td>
    <td>
    	<?
    		if($tipo == 'E'){
    			echo($total_providos);
    		}else echo($total_concursados);
    	?>
    </td>
</tr>
<tr>
	<td  align='right' class="SubTituloDireita">Total Restante</td>
    <td>
    	<?    		
    	echo($total_lancamentoauto['lancamentoauto'] - ($total_providos + $total_concursados));    	
    	?>
    	</td>
</tr>

--><?
if($habil == 'S'){
?>
<tr bgcolor="#DCDCDC">
      <td></td>
      <td colspan="4"> 	  
  	  	<input type="button" class="botao" name="btassociar" value="Novo"  onclick="location.href='reuni.php?modulo=principal/cadedital&acao=A&tipo=<?=$tipo?>&lan=<?=$lan?>&lanid=<?=$lanid?>'">
  	  	<input type="button" class="botao" name="btassociar" value="Voltar"  onclick="window.close(); window.opener.focus();"> 
    </td>
 </tr>
<?	
}
?>

</table>
</form>
	
	<?php 
	
	$sql = "SELECT 
			d.monid,
			d.monnumeditalport,
			d.mondteditalport,
			d.monnumdoueditalport,
			d.mondtdoueditalport,
			d.monnumvagas				
			FROM  reuni.dadosmonitoramento	d
			WHERE  
			d.lanid = ".$lanid." AND
			d.monano = '".$ano."' AND
			d.montipo = '".$tipo."' AND
			d.monstatus = 'A'
			";
	$dados = $db->carregar($sql);
	if( $dados ){		
		foreach($dados as $chave => $val){	
			$acao = "<center><a  href=\"reuni.php?modulo=principal/cadedital&acao=A&monid=".$val['monid']."&evento=A&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."\">
								<img src=\"/imagens/alterar.gif \" border=0 title=\"Visualizar\"></a>
								<a href=\"reuni.php?modulo=principal/listareditais&acao=A&monid=".$val['monid']."&evento=E&tipo=".$tipo."&lan=".$lan."&lanid=".$lanid."\">
								<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\"></a>
								</center>
								";
			$dados_array[$chave] = array("acao" => $acao, 
										 "numedital" => $val['monnumeditalport']." ",											 
			 							 "dtedital" => formata_data($val['mondteditalport']), 
										 "numDOU" => $val['monnumdoueditalport']." ",
			 							 "dtDOU" => formata_data($val['mondtdoueditalport']),
										 "vagas" => $val['monnumvagas']." ");
		}
		 $cabecalho = array("Ação", "Número ".$titulo2, "Data de Publicação", "Número do DOU", "Data de Publicação","Número de Vagas");
   		 $db->monta_lista_array($dados_array, $cabecalho, 50, 20, '', 'center', '');
	}else{
		 $db->monta_lista_array("", $cabecalho, 50, 20, '', 'center', '');
	}	
	?>
	
<script type="text/javascript">

function submeter(evento, indid){		
		document.getElementById('evento').value = evento;		
		document.formulario.submit();
}

function alterar(evento, monid){		
	document.getElementById('evento').value = evento;
	document.getElementById('monid').value = monid;
	document.formulario.submit();
}

function atualizar(campo, totais_tipoEP){	
	alert(campo);
	alert(totais_tipoEP);return;
	window.opener.document.getElementsByName(campo)[0].value = Number(window.opener.document.getElementsByName(campo)[0].value) - 1;
	window.opener.document.getElementById(totais_tipoEP).innerHTML = Number(window.opener.document.getElementById(totais_tipoEP).innerHTML) -1;		
}

function limpar(){	
	document.getElementById('evento').value = '';	
	document.formulario.submit();		
}

function graficohistorica(indid)
{
	var janela = window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/graficohistorica&acao=A&indid='+indid,'blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	janela.focus();
}


</script>