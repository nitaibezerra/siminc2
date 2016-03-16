<?php 
header("Content-Type: text/html; charset=ISO-8859-1"); 
include_once(APPRAIZ.'includes/classes/dateTime.inc');
?>
<?php 
//dbg($_SESSION);
session_start();
function listaItens(){
	$i='1';
	global $db;
	$sql = "SELECT 
				p.prsid as prsid,
				p.prsnumeroprocesso as processo, 
				p.prsnumconvsape as convenio, 
				p.prsanoconvsape as ano,
				date(p.prsdata) as data
			FROM
				cte.projetosape p 
			WHERE
				p.inuid = '".$_SESSION['inuid']."' AND
				p.prstipo = 'original'
			GROUP BY p.prsid, processo, convenio, ano, data
			ORDER BY processo";
//	ver($sql);
	echo    "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" width=\"100%\">
					    	<tr>
					    		<td colspan='5' bgcolor='#f7f7f7' aling='center' class='SubTituloCentro' l>
					    			Lista de Projetos FNDE / Convênios
					    		</td>
					    	</tr>
					    	<tr bgcolor='#dcdcdc'>
					    		<td><label align='center'><strong>Ações</strong></label></td>
					    		<td><label align='center'><strong>Número de Processo SAPE</strong></label></td>
					    		<td><label align='center'><strong>Número de Convênio SIAFI</strong></label></td>
					    		<td><label align='center'><strong>Ano de Convênio SIAFI</strong></label></td>
					    		<td><label align='center'><strong>Data de Envio de Processo para a SAPE</strong></label></td>
					    	</tr>";
	if (is_array($db->carregar($sql))) {
	foreach($db->carregar($sql) as $dado){
		$convenio = $dado['convenio'];
		$ano = $dado['ano'];
		$data2 = new Data();
		$data = $data2->formataData($dado['data'],"DD/MM/YYYY");
		if($i%2 == 0){
			$bgcolor = '#f7f7f7';
		}else{
			$bgcolor = '';
		}
		$i = $i+1;
		if($convenio == ''){
			$html1 = "<tr bgcolor='".$bgcolor."' class='linha'>
						<td align='left' width='60px'>
						   <img id='".$dado['prsid']."' name='".$dado['prsid']."' align='absmiddle' src='/imagens/mais.gif' onclick='carregaDiv(this.id);'/>
						   <img id='X_".$dado['prsid']."' name='X_".$dado['prsid']."' align='absmiddle' src='/imagens/excluir.gif' onclick='excluirCampo(".$dado['prsid'].");'/>";
		}else{
			$html1 = "<tr bgcolor='".$bgcolor."'>
						<td align='left' width='60px'>
						   <img id='".$dado['prsid']."' name='".$dado['prsid']." align='absmiddle' src='/imagens/mais.gif' onclick='carregaDiv(this.id);'/>";
		}
	    echo $html1."</td>
	         	<td align='left'>
	         		<input type='hidden' id='hiddenp_".$dado['prsid']."' name='hiddenp_".$dado['prsid']."' value='".$dado['processo']."'/>
	         		".$dado['processo']."
	         	</td>
	         	<td align='left'>
	         		<input type='hidden' id='hiddenc_".$dado['prsid']."' name='hiddenc_".$dado['prsid']."' value='".$convenio."'/>
	         		<input onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"  type='text' id='txc_".$dado['prsid']."' name='txc_".$dado['prsid']."' title='' value='".$convenio."' style='text-align: left; width: 13ex;' class='normal' maxlength='' size='' name='' >
	         	</td>
	         	<td align='left'>
	         		<input type='hidden' id='hiddena_".$dado['prsid']."' name='hiddena_".$dado['prsid']."' value='".$ano."'/>
	         		<input onkeyup=\"this.value=mascaraglobal('[#]',this.value);\" type='text' id='txa_".$dado['prsid']."' name='txa_".$dado['prsid']."' title='' value='".$ano."' style='text-align: left; width: 13ex;' class='normal' maxlength='' size='' name='' >
	         	</td>
	         	<td align='left'>
	         		".$data."
	         	</td>
	    	  </tr>
	    	  <tr bgcolor=''>
	    	  	<td></td>
	    	  	<td colspan='4' >
		   		    <div id='div_".$dado['prsid']."' style='display:none'></div>
	    	  	</td>
	    	  </tr>
	    	  ";
	    }
	}
    $dado = $db->carregar($sql);
    echo "<tr bgcolor='#dcdcdc' aling='center' width='700px'>
    		<td colspan='5'align='left'>Total de Registros: ";
    			echo count($dado); 
    			echo "</td>
    	  </tr>";
    echo  "<tr>
    	  	<td align='right' colspan='5'>
    			<input type='button' id='Gravar' value='Gravar' name='Gravar' onclick='atualizarTudo();' '>
    	  	</td>
    	  </tr>
    	  </table>";
}
if ($_REQUEST['carregaDiv'] == "true"){
	function carregaItensDiv($prsid){
		global $db;
		$sql = "SELECT 
					pss.pssid as pssid,
				    si.sbadsc as descricao,
				    si.sbaid as sbaid, 
				    si.sbaporescola as sbaporescola
				FROM 
					cte.subacaoindicador si
				INNER JOIN cte.projetosapesubacao pss on pss.sbaid = si.sbaid
				WHERE pss.prsid = ".$prsid."
				";
//		dbg($db->carregar($sql));
//		$db->carregar($sql);
		if(is_array($db->carregar($sql))){
			$i = 0;
			$ContDiv = "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\" width=\"100%\">
					    	<tr bgcolor='#dcdcdc'>
					    		<td><label align='center'><strong>Ação</strong></label></td>
					    		<td  colspan='2'><label align='center'><strong>Subação</strong></label></td>
					    		<td><label align='center'><strong>Valor da Subação (R$)</strong></label></td>
					    	</tr>";
			foreach($db->carregar($sql) as $dados){
				if($dados['sbaporescola'] == 'f'){
					$sqlValor = "SELECT    
									sum(cos.cosqtd * cos.cosvlruni ) AS valor
								FROM 
								    cte.subacaoindicador sba
								INNER JOIN cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = 2008
								INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = 2008
								WHERE sba.sbaid = '".$dados['sbaid']."'
								GROUP BY sba.sbaid";
				}else{
					$sqlValor = "SELECT 
									sum(cos.cosvlruni * ecs.ecsqtd) AS valor
								FROM 
								    cte.subacaoindicador sba
								INNER JOIN
								    cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = 2008
								INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid
								WHERE sba.sbaid = '".$dados['sbaid']."' 
								GROUP BY sba.sbaid";
				}
				$valor = $db->pegaUm($sqlValor);
				$valor = number_format($valor,2,',','.');
				if($i%2 == 0){
					$bgcolor = '#f7f7f7';
				}else{
					$bgcolor = '';
				}
				$ContDiv = $ContDiv."<tr bgcolor='".$bgcolor."'>
										<td><img id='".$dados['pssid']."' name='".$dados['pssid']."' align='absmiddle' src='/imagens/excluir.gif' onclick='excluirSub(this.id);'/></td>
										<td colspan = '2'><Label>".$dados['descricao']."</label></td>
										<td align='right'><Label>".$valor."</label></td>
									 </tr>";
				$i = $i+1;
			}
			$ContDiv = $ContDiv."</table>";
		}
		echo $ContDiv;
	} 
	carregaItensDiv($_REQUEST['prsid']);
	die();
}

if($_REQUEST['atualizarTudo'] == "true"){
	function atualizarTudo(){
		global $db;
		$sql = "SELECT 
					p.prsid as prsid,
					p.prsnumeroprocesso as processo, 
					p.prsnumconvsape as convenio, 
					p.prsano as ano,
					p.prsdata as data,
					spt.sptid as sptid
				FROM
					cte.projetosape p 
				INNER JOIN cte.projetosapesubacao pss    ON pss.prsid = p.prsid
				INNER JOIN cte.subacaoindicador si       ON si.sbaid  = pss.sbaid
				INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = si.sbaid
	    	    WHERE
	    	   		 p.inuid = '".$_SESSION['inuid']."'
	    	    GROUP BY p.prsid, processo, convenio, ano, data, sptid
	    	    ORDER BY processo";
		foreach($db->carregar($sql) as $dados){
			$hiddenp = $_POST['hiddenp_'.$dados['prsid']];
			$hiddenc = $_POST['hiddenc_'.$dados['prsid']] == '' ? 'null' : $_POST['hiddenc_'.$dados['prsid']];
			$txc = $_POST['txc_'.$dados['prsid']] == '' ? 'null' : $_POST['txc_'.$dados['prsid']];
			$hiddena = $_POST['hiddena_'.$dados['prsid']] == '' ? 'null' : $_POST['hiddena_'.$dados['prsid']];
			$txa = $_POST['txa_'.$dados['prsid']] == '' ? 'null' : $_POST['txa_'.$dados['prsid']];
			if($txc!=$hiddenc || $txa!=$hiddena){
				if($txc!='null' && $txa!='null'){
					$sqlUP = "UPDATE cte.projetosape 
							  SET   prsnumconvsape = ".$txc.", 
							  		prsanoconvsape = ".$txa."
							  WHERE prsnumeroprocesso = ".$hiddenp."
							  AND inuid =".$_SESSION['inuid'];
					$db->executar($sqlUP);
					$sqlUP = "UPDATE cte.subacaoparecertecnico
							  SET 	ssuid = 7
							  WHERE sptid = ".$dados['sptid'];
					$db->executar($sqlUP);
				}else{
					echo 'false';
					die();
				}
			}
		}
//		dbg($_POST);
	}
//	echo 'atualizando';
	$db->commit();
	atualizarTudo();
//	die();
}

if($_REQUEST['exluirSub'] == "true"){
	function excluirConv($pssid){
		global $db;
		$sqlSub = "DELETE FROM cte.projetosapesubacao
				   WHERE pssid = '".$pssid."' RETURNING true";
		$result = $db->executar($sqlSub);
		return ($result);
	} 
	excluirConv($_REQUEST['pssid']);
	listaItens();
	$db->commit();
	die();
}

if($_REQUEST['exluirConv'] == "true"){
	function excluirConv($prsid){
		global $db;
		$sql = "SELECT
					cx.arqid as arqid
				FROM
					cte.projetosape p 
				INNER JOIN cte.convenioxml cx ON cx.prsid = p.prsid
				WHERE
					p.prsid = '".$prsid."'
				GROUP BY p.prsid, cx.arqid
				ORDER BY p.prsid";
		$consulta = $db->carregar($sql);
		if(is_array($consulta)){
			foreach($consulta as $dados){
				$sqlConvXML = "DELETE FROM cte.convenioxml WHERE prsid = '".$prsid."'";
				$db->executar($sqlConvXML);
				$sqlArq = "DELETE FROM public.arquivo WHERE arqid = '".$dados['arqid']."'";
				$db->executar($sqlArq);
				$sqlSub = "DELETE FROM cte.projetosapesubacao
						   WHERE prsid = '".$prsid."'";
				$db->executar($sqlSub);
				$sql="DELETE FROM cte.projetosape 
					  WHERE prsid = '".$prsid."'";
				$db->executar($sql);
			}
			$db->commit();
		}
	}
	excluirConv($_REQUEST['prsid']);
	listaItens();
	die();
}

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=encoding">
<title>Gerencia de Projetos</title>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../brasilpro/includes/par_subacao.css"/>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
</head>
    <body>
    <!-- CARREGANDO DADOS AJAX -->
		<div id="loader-container" style="display:none;">
	    	<div id="loader">
	    		<img src="../imagens/wait.gif" border="0" align="middle">
	    		<span>Aguarde! Carregando Dados...</span>
	    	</div>
	    </div>
	    <form method="post" id="formListaProjetos" name="formListaProjetos" action="?modulo=principal/par_listaprojetos&acao=A">
	    	<div id="mainDiv">
			    <?php
			    	listaItens();
				?>
			</div>
	    </form>
    </body>
    
<script type="text/javascript">

	function carregaDiv(prsid){
		obDiv = document.getElementById('div_'+prsid);
		obImg = document.getElementById(prsid);
		if (obDiv.style.display == 'none'){
			obDiv.style.display = 'block'
			obImg.src = '/imagens/menos.gif'	
			if (obDiv.innerHTML == ''){
				carregaItensDiv(prsid);
				}
			}else{
				obDiv.style.display = 'none'
				obImg.src = '/imagens/mais.gif'
				}
	}
	
	function carregaItensDiv(prsid){
		$('loader-container').show(); 
		new Ajax.Request('?modulo=principal/par_listaprojetos&acao=A&prsid='+prsid+'&carregaDiv=true',{
			method: 'post',
			parameters: $('formListaProjetos').serialize('true'),
			onComplete: function(res){
			   obDiv = document.getElementById('div_'+prsid);
				obDiv.innerHTML = res.responseText;
				$('loader-container').hide();
			}
		});	
	};

	function atualizarTudo(){
		$('loader-container').show(); 
		new Ajax.Request('?modulo=principal/par_listaprojetos&acao=A&atualizarTudo=true',{
			method: 'post',
			parameters: $('formListaProjetos').serialize('true'),
			onComplete: function(res){
				if(res.responseText == 'false'){
					alert('Esta operação não pode ser concluída. Convênio e ano deve ser preenchido.');
				}else{
					obDiv = document.getElementById('mainDiv');
					obDiv.innerHTML = (res.responseText);
					alert('Itens Gravados');
				}
				$('loader-container').hide();
			}
		});	
	}
	
	function excluirCampo(prsid){
		if(confirm('Tem certeza que deseja excluir este projeto/convênio? Essa ação pode ser irreverssivel.')){
			$('loader-container').show(); 
			new Ajax.Request('?modulo=principal/par_listaprojetos&acao=A&prsid='+prsid+'&exluirConv=true',{
				method: 'post',
				parameters: $('formListaProjetos').serialize('true'),
				onComplete: function(res){
					obDiv = document.getElementById('mainDiv');
					obDiv.innerHTML = (res.responseText);
					$('loader-container').hide();
					alert('Projeto excluído');
				}
			});	
		}
	}
	
	function excluirSub(pssid){
		if(confirm('Tem certeza que deseja desvincular esta Subação? Essa ação pode ser irreverssivel.')){
			$('loader-container').show(); 
			new Ajax.Request('?modulo=principal/par_listaprojetos&acao=A&pssid='+pssid+'&exluirSub=true',{
				method: 'post',
				parameters: $('formListaProjetos').serialize('true'),
				onComplete: function(res){
					obDiv = document.getElementById('mainDiv');
					obDiv.innerHTML = (res.responseText);
					$('loader-container').hide();
					alert('Subação excluída');
				}
			});	
		}
	}
	
</script>
</html>