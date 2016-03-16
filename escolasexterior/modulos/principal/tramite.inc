<?php

//verifica session
verificaSessao();
include APPRAIZ . 'includes/workflow.php';

//verifica permissão
$vPermissao = 'N';
$vpflcod = arrayPerfil();
if( in_array(PERFIL_SUPER_USUARIO, $vpflcod) ){
	$vPermissao = 'S';
}
if($_SESSION['eexid']){
$docid = criaDocumento( $_SESSION['eexid'] ); 
    
}
if($docid) {
	$documento = wf_pegarDocumento( $docid );
	$atual = wf_pegarEstadoAtual( $docid );
	$historico = wf_pegarHistorico( $docid );
} 


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo = "ESCOLAS NO EXTERIOR";

$dsctitulo = 'TRÂMITE<BR>- Verifique abaixo as pendências para enviar o documento.';


montaAbasEscolasExterior();

monta_titulo( $titulo, $dsctitulo );

cabecalhoEE($_SESSION['peeid']);

function buscaDocumentosPendentes(){
	global $db;
	$sql = "
		select tpdid, tpddsc
		from escolasexterior.tipodocumento 
		where tpdid < 8
	";
	$arrayResultado = $db->carregar( $sql );
	foreach ($arrayResultado as $key => $value) {
		$arrayPendentes[ $value['tpdid'] ] = $value['tpddsc'];
	}

	$sql = "
		select t.tpdid, td.tpddsc
		from escolasexterior.tpdocescolaext t
		join escolasexterior.tipodocumento td on td.tpdid = t.tpdid
		where t.eexid = '{$_SESSION['eexid']}'
	";
	$resultado = $db->carregar( $sql );

	if( $resultado )
	foreach ($resultado as $key => $value) {
		unset($arrayPendentes[ $value['tpdid'] ]);
	}
	
	return $arrayPendentes;
}

?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

 
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form id="formulario" name="formulario" method="post"  >

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td bgcolor="#e9e9e9" align="center">
				<b>Pendências</b>
			</td>
		</tr>
		<tr>
			<td align="center">
					
					<br>
					
					<?php 
					$documentosPendentes = buscaDocumentosPendentes();
					$contador = 1;
					foreach( $documentosPendentes as $index => $value ){ ?>
						<font color="red">
							<b><?=$contador?>. Aba Documentos: Favor anexar o tipo de documento <?=$value?>.</b>
						</font><br>
					<?php $contador++;} ?>

					<br>
					
					<?php
			
							if($docid){
								wf_desenhaBarraNavegacao( $docid , array("eexid" => $_SESSION['eexid']) );
							} 
					?>
				
			</td>
		</tr>
	</table>
    <table class="tabela" cellspacing="0" cellpadding="3" align="center">
		<thead>
			<tr>
				<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
					<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
					<div><?php  echo $documento['docdsc'];; ?></div>
				</td>
			</tr>
			<?php if ( count( $historico ) ) : ?>
				<tr>
					<td style="width: 20px;"><b>Seq.</b></td>
					<td style="width: 200px;"><b>Onde Estava</b></td>
					<td style="width: 200px;"><b>O que aconteceu</b></td>
					<td style="width: 90px;"><b>Quem fez</b></td>
					<td style="width: 120px;"><b>Quando fez</b></td>
					<td style="width: 17px;">&nbsp;</td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php $i = 1; ?>
		<?php foreach ( $historico as $item ) : ?>
			<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
			<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td align="right"><?=$i?>.</td>
				<td style="color:#008000;">
					<?php echo $item['esddsc']; ?>
				</td>
				<td valign="middle" style="color:#133368">
					<?php echo $item['aeddscrealizada']; ?>
				</td>
				<td style="font-size: 6pt;">
					<?php echo $item['usunome']; ?>
				</td>
				<td style="color:#133368">
					<?php echo $item['htddata']; ?>
				</td>
				<td style="color:#133368; text-align: center;">
					<?php if( $item['cmddsc'] ) : ?>
						<img
							align="middle"
							style="cursor: pointer;"
							src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
							onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
						/>
					<?php endif; ?>
				</td>
			</tr>
			<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td colspan="6">
					<div >
						<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
					</div>
				</td>
			</tr>
			<?php $i++; ?>
		<?php endforeach; ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="6">
				Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
			</td>
		</tr>
	</table>
	
</form>

</body>

</html>



