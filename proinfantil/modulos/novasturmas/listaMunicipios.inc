<?php
if($_SESSION['exercicio']=='2011'){
	echo "<script>alert('Programa Novas Turmas não disponível no ano de exercício 2011!'); 
		 document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
		 </script>";
	exit();
}

if($_REQUEST['download'] == 'manual'){
	
	$caminho = APPRAIZ. "www/proinfantil/documentos/Manual_Novas_Turmas_Educacao_Infantil.pdf";
	if ( !is_file( $caminho ) ) {
        echo"<script>
            	alert('Arquivo não encontrado na pasta.');
            	window.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
            </script>";
    	exit();
	} 
    $filename = "Manual_Novas_Turmas_Educacao_Infantil.pdf";
    header( 'Content-type: pdf' );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    echo"<script>window.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';</script>";	
}

unset($_SESSION['proinfantil']['turid']);

$perfis = pegaPerfil($_SESSION["usucpf"]);

if($perfis){
	if(in_array(EQUIPE_MUNICIPAL,$perfis)){ 
		$sql = "SELECT 		muncod 
				FROM 		proinfantil.usuarioresponsabilidade 
				WHERE 		usucpf = '{$_SESSION["usucpf"]}' AND pflcod = 549 AND rpustatus = 'A'";
		$muncodDoUsuario = $db->pegaUm( $sql );
	}
}

if($_SESSION['proinfantil']['muncod']) unset($_SESSION['proinfantil']['muncod']);

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<table class="tabela" align="center" cellspacing="0" cellpadding="3" width="100%">
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar"/>
		</td>
		<td class="TituloTela">Novas Turmas</td>
		<td class="subtitulodireita" valign="top">
			<a style="cursor: pointer; color: blue;" onclick="window.location='proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A&download=manual'"><b><img src="../imagens/pdf.gif" width="18px" border="0"> Manual Novas Turmas Educação Infantil</b></a>
		</td>
	</tr>
</table>

<br/>
<?php 

$arr2013 = Array();

if($_SESSION['exercicio'] < '2013'){
	$arr2013 = Array(13362, 13865);
}

$db->cria_aba($abacod_tela,$url,$parametros, $arr2013); 

$titulo_modulo = "Lista de Municípios";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

if($_POST){
	extract($_POST);
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	$('.abrirMunicipio').click(function(){
		
		if( $('[name="anoatual"]').val() >= '2013'  ){
			document.location.href = 'proinfantil.php?modulo=novasturmas/informarMatTurmaMunicipio&acao=A&muncod='+this.id;
		} else {
			document.location.href = 'proinfantil.php?modulo=novasturmas/questionarioAtendimento&acao=A&muncod='+this.id;
		}
	});
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == '' && $('[name=esdid]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}	
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
	});
	$('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
	});
});
</script>
<?php if(!in_array(EQUIPE_MUNICIPAL,$perfis)){ ?>
<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="anoatual" value="<?=$_SESSION['exercicio']; ?>">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">UF:</td>
			<td>
				<?php 
				$sql = "SELECT		estuf as codigo,
									estdescricao as descricao
						FROM		territorios.estado
						ORDER BY 	estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município:</td>
			<td>
				<?php echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		<?php if($_SESSION['exercicio']>='2012'){?>
		<tr>
			<td class="subtituloDireita">Situação:</td>
			<td>
				<?php 
				$sql = "SELECT 		esdid as codigo,
									esddsc as descricao
						FROM		workflow.estadodocumento
						WHERE		tpdid = ".WF_TPDID_PROINFANTIL_NOVASTURMAS;
				$db->monta_combo('esdid', $sql, 'S', 'Selecione..', '', '', '', '');
				?>
			</td>
		</tr>
		<?php } ?>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<?php } else { ?>
	<input type="hidden" name="anoatual" value="<?=$_SESSION['exercicio']; ?>">
<?php 
}

if($estuf){
	$arWhere[] = "mun.estuf = '{$estuf}'";
}

if($mundescricao){
	if( is_numeric( $mundescricao ) ){
		$arWhere[] = "mun.muncod = '".trim($mundescricao)."'";
	} else {
		$arWhere[] = "UPPER( removeacento(mun.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($mundescricao) ) )."%'";
	}
}

if($muncodDoUsuario){
	$arWhere[] = "mun.muncod = '{$muncodDoUsuario}' ";
}

switch($_SESSION['exercicio']){
	case '2012':
		if($esdid){
			$arWhere[] = "esd.esdid = {$esdid}";
		} 
		$select = ", esd.esddsc";		
		$join = "LEFT JOIN	workflow.documento doc on doc.docid = ntm.docid
				 LEFT JOIN	workflow.estadodocumento esd on esd.esdid = doc.esdid";
		$group = ",esd.esddsc";
		$cabecalho = array('Ação', 'UF', 'Município', 'Situação');

		break;
	default:
		if($esdid){
			$arWhere[] = "tab.esdid = {$esdid}";
		}
		$select = ", SUM(tab.emcadastramento) as emcadastramento,
				   SUM(tab.emanalise) as emanalise,
				   SUM(tab.emdiligencia) as emdiligencia,
				   SUM(tab.aguardandopagamento) as aguardandopagamento,
				   SUM(tab.pagamentoefetuado) as pagamentoefetuado";
		$join = "LEFT JOIN	(SELECT 	ntw.muncod, doc.esdid,
										CASE WHEN doc.esdid = 535 THEN COUNT(ntw.turid) ELSE '0' END as emcadastramento, 
										CASE WHEN doc.esdid = 536 THEN COUNT(ntw.turid) ELSE '0' END as emanalise, 
										CASE WHEN doc.esdid = 587 THEN COUNT(ntw.turid) ELSE '0' END as emdiligencia, 
										CASE WHEN doc.esdid = 586 THEN COUNT(ntw.turid) ELSE '0' END as aguardandopagamento, 
										CASE WHEN doc.esdid = 599 THEN COUNT(ntw.turid) ELSE '0' END as pagamentoefetuado
				 FROM	    	proinfantil.novasturmasworkflowturma ntw
				 inner join workflow.documento doc ON doc.docid = ntw.docid
				 inner join proinfantil.turma t on t.turid = ntw.turid and t.turstatus = 'A' and t.turano = '{$_SESSION['exercicio']}'
				 GROUP BY	 	doc.esdid, ntw.muncod) as tab ON tab.muncod = ntm.muncod";
		$cabecalho = array('Ação', 'UF', 'Município','Em Cadastramento','Em Análise','Em Diligência','Aguardando Pagamento','Pagamento Efetuado');

		break;
}

$sql = "SELECT		'
					<img src=\"../imagens/alterar.gif\" id=\"' || mun.muncod ||'\" class=\"abrirMunicipio\" style=\"cursor:pointer;\"/>
					' as acao,
					mun.estuf,
					mun.mundescricao
					$select
		FROM		territorios.municipio mun
		LEFT JOIN 	proinfantil.novasturmasmunicipios ntm on ntm.muncod = mun.muncod
					$join
					".(is_array($arWhere) ? ' where '.implode(' and ', $arWhere) : '')."
		GROUP BY 	mun.muncod, mun.estuf, mun.mundescricao $group
		ORDER BY 	mun.estuf, mun.mundescricao";
//ver( simec_htmlentities($sql) );
$db->monta_lista($sql, $cabecalho, 50, 10, '', '', '', '', array('60', '40', null, 160));

?>