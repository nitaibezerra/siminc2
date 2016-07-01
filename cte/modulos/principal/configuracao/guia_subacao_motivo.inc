<?php
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}

function gravarmotivomonitoramento($dados) {
	global $db;
	if($dados['motivo']['inserir']) {
		foreach($dados['motivo']['inserir'] as $momdsc) {
			$sql = "INSERT INTO cte.motivomonitoramento(
        		    ppsid, momdsc)
    				VALUES ('".$dados['ppsid']."', '".$momdsc."');";
			$db->executar($sql);
		}
	}
	if($dados['motivo']['atualizar']) {
		foreach($dados['motivo']['atualizar'] as $momid => $momdsc) {
			$sql = "UPDATE cte.motivomonitoramento
   			SET momdsc='".$momdsc."'
 			WHERE momid='".$momid."'";
			$db->executar($sql);
		}
	}
	$db->commit();
	echo "<script>
			alert('Motivos cadastrados com sucesso.');
			window.location='?modulo=principal/configuracao/guia_subacao_motivo&acao=A&ppsid=".$dados['ppsid']."';
		  </script>";
	exit;
}


function deletarmotivomonitoramento($dados) {
	global $db;
	$sql = "DELETE FROM cte.motivomonitoramento WHERE momid='".$dados['momid']."'";
	$db->executar($sql);
	$db->commit();
}

function inserirmotivomonitoramento($dados) {
	global $db;
	$sql = "INSERT INTO cte.motivomonitoramento(ppsid, momdsc)
    		VALUES ('".$dados['ppsid']."', '".$dados['momdsc']."');";
	$db->executar($sql);
	$db->commit();
	exit;
}
function atualizarmotivomonitoramento($dados) {
	global $db;
	$sql = "UPDATE cte.motivomonitoramento
   			SET momdsc='".$dados['momdsc']."'
 			WHERE momid='".$dados['momid']."'";
	$db->executar($sql);
	$db->commit();
	exit;
}

function listarmotivomonitoramento($dados) {
	global $db;
	$sql = "SELECT '<center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"editarmotivomonitoramento(this,'||momid||');\"> <img src=\"/imagens/excluir.gif\" border=0 style=\"cursor:pointer;\" onclick=\"deletarmotivomonitoramento('||momid||');\"></center>' as acoes, momdsc FROM cte.motivomonitoramento WHERE ppsid='".$dados['ppsid']."'";
	$cabecalho = array("Ações","Motivos");
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '100%','N');
	exit;
}


$abas = array(0 => array("id"=>"1","descricao"=>"Dados","link"=>"/cte/cte.php?modulo=principal/configuracao/guia_subacao&acao=A&ppsid=".$_REQUEST['ppsid']),
			  1 => array("id"=>"2","descricao"=>"Motivos","link"=>"/cte/cte.php?modulo=principal/configuracao/guia_subacao_motivo&acao=A&ppsid=".$_REQUEST['ppsid']));
echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

if(!$_REQUEST['ppsid']) {
	echo "<script>alert('Parametros não encontrados');window.close();</script>";
	exit;
}

$sql = '
select
    *
    ,ps.indid as indicadorsubacao
    ,ps.indqtdporescola as qtdporescola
from
    cte.proposicaosubacao ps
left join
    cte.proposicaoacao pa on pa.ppaid = ps.ppaid
left join
    cte.criterio c on c.crtid = pa.crtid
inner join
    cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = \'A\'
inner join
    cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = \'A\'
inner join
    cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\'
where
	ps.ppsid = '. $_REQUEST['ppsid'];

$subacao = $db->pegaLinha($sql);

$sql = sprintf(
	"select ps.ppsid
	from cte.proposicaosubacao ps
	inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
	inner join cte.criterio c on c.crtid = pa.crtid
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	inner join cte.instrumento it on it.itrid = d.itrid
	order by it.itrdsc, d.dimcod, ad.ardcod, i.indcod, c.ctrord, ps.ppsordem"
);


?>
<html style="overflow: scroll;">
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script language="JavaScript" src="../includes/prototype.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
<script type="text/javascript">
function ajaxatualizar(params,iddestinatario) {	
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				//alert(resp.responseText);
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
}

function inserirmotivomonitoramento() {
	var tabela = document.getElementById('listamotivos');
	var linha = tabela.insertRow(1);
	var cell1 = linha.insertCell(0);
	var cell2 = linha.insertCell(1);
	cell1.innerHTML = "<input type='hidden' name='motivo[inserir][]' value='"+document.getElementById('momdsc').value+"'> <center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"editarmotivomonitoramento(this);\"> <img src=\"/imagens/excluir.gif\" border=0 style=\"cursor:pointer;\" onclick=\"deletarmotivomonitoramento('||momid||');\"></center>";
	cell2.innerHTML = document.getElementById('momdsc').value;
	document.getElementById('momdsc').value='';
}
function gravaredicaomotivomonitoramento(obj) {
	var linha = obj.parentNode.parentNode;
	var input = obj.parentNode.firstChild;
	var input2 = linha.cells[0].firstChild;
	linha.cells[1].innerHTML = input.value;
	input2.value = input.value;
}
function editarmotivomonitoramento(obj) {
	var linha = obj.parentNode.parentNode.parentNode;
	linha.cells[1].innerHTML = "<input type='text' value='"+linha.cells[1].innerHTML+"' size='40'> <input type='button' value='Ok' onclick='gravaredicaomotivomonitoramento(this);'>"; 
}

function deletarmotivomonitoramento(obj) {
	var conf = confirm("Deseja realmente excluir o motivo?");
	if(conf) {
		var linha = obj.parentNode.parentNode.parentNode;
		var tabela = obj.parentNode.parentNode.parentNode.parentNode;
		tabela.deleteRow(linha.rowIndex);
		var input = obj.parentNode.parentNode.firstChild;
		if(input.id) {
			ajaxatualizar('exec_function=deletarmotivomonitoramento&momid='+input.id,'');
		}
	}
}
</script>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="exec_function" value="gravarmotivomonitoramento"/>
	<input type="hidden" name="ppsid" value="<?= $_REQUEST["ppsid"] ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col style="width: 125px;"/><col/>
		</colgroup>
		<tbody>
		<tr>
			<td align='right' class="SubTituloDireita">Dimensão:</td>
			<td><b><?= $subacao['dimcod'] ?></b> <?= $subacao['dimdsc'] ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Área:</td>
			<td><b><?= $subacao['dimcod'].'.'.$subacao['ardcod'] ?></b> <?= $subacao['arddsc'] ?></td>
		</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Indicador:</td>
				<td><b><?= $subacao['dimcod'] .'.'. $subacao['ardcod'] .'.'. $subacao['indcod'] ?></b> <?= $subacao['inddsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Critério:</td>
				<td><b><?= abs( $subacao['ctrord'] - 5 ) ?></b> <?= $subacao['crtdsc'] ?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Ação:</td>
				<td><b><?= $subacao['ppadsc'] ?></b></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Subação:</td>
				<td>
					<?php
					$ppsdsc = $_REQUEST["ppsdsc"] ? $_REQUEST["ppsdsc"] : $subacao["ppsdsc"];
					echo $ppsdsc;
					?>
				</td>
			</tr>
			<tr>													
				<td class="SubTituloCentro" colspan="2">Cadastrar motivo</td>
			</tr>

			<tr>													
				<td align='right' class="SubTituloDireita">Motivo:</td>
				<td><img src='../imagens/gif_inclui.gif' border='0' align='absmiddle' onclick='inserirmotivomonitoramento();'> 
					<?
					echo campo_texto( 'momdsc', 'N', 'S', '', 70, 150, '', '','','','','id="momdsc"');
					?> 
				</td>
			</tr>
			<tr>
			<td colspan='2'>
				<table class="listagem" width="100%" id="listamotivos" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td class="SubTituloCentro" style='width:100px;'>Ações</td>
						<td class="SubTituloCentro">Motivo</td>
					</tr>
					<?
					$sql = "SELECT * FROM cte.motivomonitoramento WHERE ppsid='".$_REQUEST['ppsid']."'";
					$motivos = $db->carregar($sql);
					if($motivos[0]) {
						foreach($motivos as $motivo) {
							echo "<tr>";
							echo "<td><input type='hidden' id='".$motivo['momid']."' name='motivo[atualizar][".$motivo['momid']."]' value='".$motivo['momdsc']."'> <center><img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" onclick=\"editarmotivomonitoramento(this);\"> <img src=\"/imagens/excluir.gif\" border=0 style=\"cursor:pointer;\" onclick=\"deletarmotivomonitoramento(this);\"></center></td>";
							echo "<td>".$motivo['momdsc']."</td>";
							echo "</tr>";
						}
					}
					?>
			</table>
		</td>
		</tr>
		<tr bgcolor="#C0C0C0">													
			<td colspan='2' align='center'><input type='button' value='Gravar' onclick="document.getElementById('formulario').submit();"></td>
		</tr>
		</tbody>
	</table>
</form>