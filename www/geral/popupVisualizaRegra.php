<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

if (!$_SESSION['usucpf']){
	die('<script>
			alert(\'Acesso negado!\');
			window.close();
		 </script>');
}

if( $_REQUEST['arqid'] ) DownloadArquivo( $_REQUEST['arqid'] );

function DownloadArquivo($arqid){
	global $db;
	
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
	$arquivo = current($db->carregar($sql));
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo['arqid']/1000) .'/'.$arquivo['arqid'];
	if ( !is_file( $caminho ) ) {
		$_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
	}
	if ( is_file( $caminho ) ) {
		$filename = str_replace(" ", "_", $arquivo['arqnome'].'.'.$arquivo['arqextensao']);
		header( 'Content-type: '. $arquivo['arqtipo'] );
		header( 'Content-Disposition: attachment; filename='.$filename);
		readfile( $caminho );
		exit();
	} else {
		die("<script>alert('Arquivo não encontrado.');window.location.href='popupVisualizaRegra.php?rgacod=".$_REQUEST['rgacod']."&mnuid=".$_REQUEST['mnuid']."';</script>");
		
	}
}

$sql = "SELECT
		  r.rgadesc,
		  r.rgacampo,
		  r.rgaregra,
		  u.usunome as demandante,
          u1.usunome as desenvolvedor,
          u2.usunome as solicitante,
          r.rgadata,
          ar.arqdescricao,
          r.arqid
		FROM 
		  seguranca.regra r 
		  left join seguranca.usuario u on u.usucpf = r.rgademandante
          left join seguranca.usuario u1 on u1.usucpf = r.rgadesenvolvedor
          left join seguranca.usuario u2 on u2.usucpf = r.rgasolicitante
          left join public.arquivo ar on ar.arqid = r.arqid
		WHERE 
			r.rgacod = ".$_GET['rgacod']."
			and r.mnuid = ".$_GET['mnuid']."
		ORDER BY r.rgaid desc";
$arRegras = $db->carregar( $sql );
$arRegras = $arRegras ? $arRegras : array();

$sql = "SELECT sisdsc FROM seguranca.sistema WHERE sisid = ".$_SESSION['sisid'];
$sisdsc = $db->pegaUm($sql);

$corTitulo = '#DCDCDC';
$corRegistro = '#F0F0EE';
//$corRegistro = '#FFFFCC';

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="/includes/prototype.js"></script>
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="notprint">
    	<tbody>
    	<tr bgcolor="#DCDCDC">
    		<td align="center" width="100%"><label class="TituloTela" style="color: rgb(0, 0, 0);">Visualizar Historico de Regras</label></td>
    	</tr>
    	<tr>
    		<td style="" align="center" bgcolor="#e9e9e9"><?=$sisdsc; ?></td>
    	</tr>
    	</tbody>
    </table>
<?foreach ($arRegras as $key => $v):?>
<table id="tblform" class="notprint" border="1" width="100%" cellspacing="0" cellpadding="2" align="center">
<tr><td>
<table id="tblform" class="notprint" border="0" width="100%" cellspacing="0" cellpadding="2" align="center">
	<tr bgcolor="#DCDCDC">
		<td colspan="4" style="text-align: center;"></td>
	</tr>
	<tr>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita"><b>Descrição:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=$v['rgadesc'] ?></td>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita" width="120px"><b>Campo:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=$v['rgacampo'] ?></td>		
	</tr>
	<tr>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita" width="120px"><b>Getor:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=$v['demandante'] ?></td>		
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita"><b>Analista:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=$v['solicitante'] ?></td>
	</tr>
	<tr>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita"><b>Desenvolvedor:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=$v['desenvolvedor'] ?></td>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita"><b>Data:</b></td>
		<td bgcolor="<?=$corRegistro; ?>"><?=formata_data($v['rgadata']); ?></td>	
	</tr>
	<?if(!empty($v['arqdescricao'])){ ?>
	<tr>
		<td bgcolor="<?=$corTitulo; ?>" class="subtitulodireita"><b>Anexos:</b></td>
		<td bgcolor="<?=$corRegistro; ?>" colspan="3"><a href="popupVisualizaRegra.php?rgacod=<?=$_REQUEST['rgacod']; ?>&mnuid=<?=$_REQUEST['mnuid']; ?>&arqid=<?=$v['arqid']; ?>"><?=$v['arqdescricao']; ?></a></td>	
	</tr>
	<?} ?>
	<tr bgcolor="<?=$corTitulo; ?>">
		<td colspan="4" style="text-align: center;"><b>Descrição da Regra</b></td>
	</tr>
	<tr>
		<td bgcolor="<?=$corRegistro; ?>" colspan="4"><?=html_entity_decode($v['rgaregra']); ?></td>
	</tr>
</table>
</td></tr></table><br>
<?endforeach; ?>
<table id="tblform" class="notprint" width="100%" cellspacing="0" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="4" style="text-align: center;">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="window.close();">
		</td>
	</tr>
</table>