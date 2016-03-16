<?php 
$perfis = pegaPerfil($_SESSION["usucpf"]);

if( $perfis[0] ){
	if( in_array( EQUIPE_MUNICIPAL, $perfis) ){ 
		$sql = "select muncod from proinfantil.usuarioresponsabilidade where usucpf = '".$_SESSION["usucpf"]."' and pflcod = ".EQUIPE_MUNICIPAL." and rpustatus = 'A'";
		$muncodDoUsuario = $db->pegaUm( $sql );		
	}
}

if($_REQUEST['download'] == 'manual'){
	
	$caminho = APPRAIZ. "www/proinfantil/documentos/Manual_Suplementacao_de_Creches_MDS.pdf";
	if ( !is_file( $caminho ) ) {
        echo"<script>
            	alert('Arquivo não encontrado na pasta.');
            	window.location.href = 'proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A';
            </script>";
    	exit();
	} 
    $filename = "Manual_Suplementacao_de_Creches_MDS.pdf";
    header( 'Content-type: pdf' );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    echo"<script>window.location.href = 'proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A';</script>";	
}

if($_SESSION['proinfantil']['mds']['muncod']) unset($_SESSION['proinfantil']['mds']['muncod']);

// dbg($_SERVER);
// die;

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar"/>
		</td>
		<td class="TituloTela">Suplementação MDS</td>
		<td class="subtitulodireita" valign="top">
			<a style="cursor: pointer; color: blue;" onclick="window.location='proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A&download=manual'"><b><img src="../imagens/pdf.gif" width="18px" border="0"> Manual Suplementação de Creches MDS</b></a>
		</td>
	</tr>
</table>

<br/>
<?php 

//if($_SESSION['baselogin']=="simec_desenvolvimento") {
//	$abacod_tela = 57559;
//}else{
//	$abacod_tela = 57568;
//}
$arMnuid = array(11113, 11932, 11114, 11127);

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

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
		document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A&muncod='+this.id;
	});
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == '' && $('[name=esdid]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}	
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaMunicipios&acao=A';
	});
	$('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
	});
});

function visualizarDadosMDS(municipio){
	return window.open('proinfantil.php?modulo=suplementacaomds/listaMDS&acao=A&municipio='+municipio,'modelo',"height=600,width=950,scrollbars=yes,top=50,left=200");
}

</script>
<?php if( !in_array( EQUIPE_MUNICIPAL, $perfis) ){ ?>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">UF</td>
			<td>
				<?php 
				$sql = "select 
							estuf as codigo,
							estdescricao as descricao
						from
							territorios.estado
						order by 
							estuf";
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município</td>
			<td>
				<?php echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">Situação</td>
			<td>
				<?php 
				$sql = "select 
							esdid as codigo,
							esddsc as descricao
						from 
							workflow.estadodocumento
						where 
							tpdid = ".WF_TPDID_SUPLEMENTACAO_MDS;
				
				$db->monta_combo('esdid', $sql, 'S', 'Selecione..', '', '', '', '');
				?>
			</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<?php } ?>
<?php 

if($estuf){
	$arWhere[] = "mun.estuf = '{$estuf}'";
}

if($mundescricao){
	$arWhere[] = "UPPER( removeacento(mun.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($mundescricao) ) )."%'";
}

if($muncodDoUsuario){
	$arWhere[] = "mun.muncod = '{$muncodDoUsuario}' ";
}

if($esdid){
	$arWhere[] = "doc.esdid = ".$esdid;
}

$sql = "select
			'<img src=\"../imagens/alterar.gif\" id=\"' || mun.muncod ||'\" class=\"abrirMunicipio\" style=\"cursor:pointer;\"/>
			<img src=\"../imagens/mds_lista.gif\" id=\"' || mun.muncod ||'\" onclick=\"visualizarDadosMDS(\'' || mun.muncod || '\')\" style=\"cursor:pointer;\"/>' as acao,
			mun.estuf,
			mun.muncod,
			mun.mundescricao,
			case when esd.esddsc is null then 'Em cadastramento' else esd.esddsc end as situacao
		from 
			territorios.municipio mun
		inner join
			proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod and dcm.cpmano = ({$_SESSION['exercicio']} - 1)
		left join 
			workflow.documento doc on doc.docid = dcm.docid
		left join 
			workflow.estadodocumento esd on esd.esdid = doc.esdid
		inner join
			proinfantil.procenso pc on pc.muncod = dcm.muncod and pc.prcano = ({$_SESSION['exercicio']} - 1)
		".(is_array($arWhere) ? ' where '.implode(' and ', $arWhere) : '')."
		group by mun.muncod, mun.estuf, mun.mundescricao, esd.esddsc
		order by 
			mun.estuf, mun.mundescricao
			";

$cabecalho = array('Ação', 'UF', 'Código IBGE', 'Município', 'Situação');
$db->monta_lista($sql, $cabecalho, 50, 10, '', '', '', '', array(60, 40, 100, null, 140));

?>