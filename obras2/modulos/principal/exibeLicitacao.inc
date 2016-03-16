<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$orgid = $_SESSION['obras2']['orgid'];
$obrid = $_SESSION['obras2']['obrid'];

////se não tiver obra retorna
//if ( empty($obrid) ){
//	die("<script>
//			alert('Faltam parametros para acessar esta tela!');
//	        history.go(-1);
//		 </script>");
//}

switch ( $_REQUEST['requisicao'] ){
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die("<script>
		        history.go(-1);
			 </script>");
}

$obra  = new Obras( $obrid );
if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';
	
//	$arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
	$arMenuBlock = array();
	
	if( $orgid == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,$arMenuBlock);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,$arMenuBlock);
	}
	
	$esdid = pegaEstadoObra( $obra->docid );
	if ( $esdid == ESDID_OBJ_LICITACAO || $esdid == ESDID_OBJ_AGUARDANDO_1_REPASSE ){
		$habilitado = true;
		$habilita 	= 'S';
	}else{
		$habilitado = false;
		$habilita 	= 'N';
	}
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);	
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
	$habilitado = false;
	$habilita 	= 'N';
	
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita 	= 'N';
}

$docid = pegaDocidObra( $obrid );

echo cabecalhoObra($obrid);
//echo '<br>';
monta_titulo( $titulo_modulo, '' );

$licitacao = new Licitacao();
$dados = $licitacao->pegaDadosPorObra( $obrid );


?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td width="100%" class="" style="font-weight: bold" valign="top"><a onclick="DownloadArquivo(10329484);" href="#">Acórdão do TCU vedando a utilização da modalidade licitatória Pregão a obras de engenharia</a></td>
    </tr>
<?


if ( empty($dados['licid']) ):
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="100%" class="subtitulocentro" style="color: red;" valign="top">Esta obra não está vinculada a nenhuma licitação.</td>
		<td>
            <?php
            // Barra de estado WORKFLOW
            if( possui_perfil(PFLCOD_CALL_CENTER)){
                wf_desenhaBarraNavegacao( $docid, array('obrid' => $obrid), array('acoes' => true) );
            }else if ( $docid ){
            	wf_desenhaBarraNavegacao($docid, array('obrid' =>  $obrid));
            }
            ?>		
		</td>
	</tr>
<?php
	if ( $habilitado ):
?>
	<tr bgcolor="#DEDEDE">
		<td colspan="2" align="center">
			<input type="button" name="botao" value="Cadastrar licitação para esta obra" onclick="window.location='?modulo=principal/cadLicitacao&acao=A';"/>
		</td>
	</tr>
<?php
	endif;
?>
</table>
<?php 
else:
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" border="0">
	<tr>
		<td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
		<td>
		<?php echo $dados['moldsc'] ?>
		</td>
		<td rowspan="4" align="right" valign="top" width="1">
            <?php
            // Barra de estado WORKFLOW
            if( possui_perfil(PFLCOD_CALL_CENTER)){
                wf_desenhaBarraNavegacao( $docid, array('obrid' => $obrid), array('acoes' => true) );
            }  else if($docid) {
                wf_desenhaBarraNavegacao($docid, array('obrid' =>  $obrid));
            }
            ?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Número da Licitação:</td>
		<td>
		<?php echo $dados['licnumero'] ?>
		</td>
	</tr>
	<tr>
		<td>Fases de Licitação</td>
	</tr>
	<tr>
		<td colspan="2" valign="top">
			<table id="faseslicitacao" width="70%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
				<thead>
					<tr bgcolor="#e9e9e9">
						<td width="70%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
						<td width="20%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>						
					</tr>
				</thead>
				<?php 
					if($dados['licid']){
						
						$faseLic = new FaseLicitacao();
						$arDados = $faseLic->listaPorLicitacao( $dados['licid'] );

						foreach ($arDados as $k => $dados){
//						while (($dados = pg_fetch_assoc($sql))){
							$flcid 				= $dados['flcid'];
							$tflid 				= $dados['tflid'];
							$tfldesc 			= $dados['tfldesc'];
							$flcrecintermotivo 	= $dados['flcrecintermotivo'];
							$flcordservnum 		= $dados['flcordservnum'];
							$flcpubleditaldtprev = formata_data($dados['flcpubleditaldtprev']);
							$flcdtrecintermotivo = formata_data($dados['flcdtrecintermotivo']);
							$flcordservdt 		= formata_data($dados['flcordservdt']);
							$flchomlicdtprev 	= formata_data($dados['flchomlicdtprev']);
							$flcaberpropdtprev 	= formata_data($dados['flcaberpropdtprev']);
							$tflordem 			= $dados['tflordem'];
							$flcmeiopublichomol = $dados['flcmeiopublichomol'];
							$flcobshomol 		= $dados['flcobshomol'];
							$arqid 				= $dados['arqid'];
							
							if($tflid ==2){
								$flcdata = $flcpubleditaldtprev;
							}
							if($tflid ==5){
								$flcdata = $flcdtrecintermotivo;
							}
							if($tflid ==6){
								$flcdata = $flcordservdt;
							}
							if($tflid ==9){
								$flcdata = $flchomlicdtprev;
							}
							if($tflid ==7){
								$flcdata = $flcaberpropdtprev;
							}
							
							$imgAnexo .= "";
							if ( $arqid ){
								$imgAnexo .= "<img src='/imagens/anexo.gif' 
												   style='cursor: pointer' 
												   border='0' 
												   title='Baixar arquivo' 
												   onclick=\"DownloadArquivo(" . $arqid . ");\"/>";
							}
							
							echo "
								<tr id=\"tr_" . $flcid . "\">
									<td>
										<input type='hidden' name='flcid[]' id='flcid_".$flcid."'  value='".$flcid."'/>
										<img onclick='visualizaFase(" . $flcid . ");' src='/imagens/consultar.gif' border='0' style='cursor:pointer;' title='Visualizar dados da fase'>
										&nbsp;
										{$imgAnexo}
										" . $tfldesc . "
									</td>
									<td>
										<input type='hidden' name='flcpubleditaldtprev[]' id='flcpubleditaldtprev_".$flcid."' value='".$flcpubleditaldtprev."'/>
										<input type='hidden' name='flcdtrecintermotivo[]' id='flcdtrecintermotivo_".$flcid."' value='".$flcdtrecintermotivo."'/>
										<input type='hidden' name='flcordservdt[]' id='flcordservdt_".$flcid."' value='".$flcordservdt."'/>
										<input type='hidden' name='flchomlicdtprev[]' id='flchomlicdtprev_".$flcid."' value='".$flchomlicdtprev."'/>
										<input type='hidden' name='flcaberpropdtprev[]' id='flcaberpropdtprev_".$flcid."' value='".$flcaberpropdtprev."'/>									
										<input type='hidden' name='flcdata[]' id='flcdata_".$flcid."' value='".$flcdata."'/>
										" . $flcdata . "
										<input type='hidden' name='flcrecintermotivo[]' id='flcrecintermotivo_".$flcid."' value='".$flcrecintermotivo."'/>
										<input type='hidden' name='flcobshomol[]' id='flcobshomol_".$flcid."' value='".$flcobshomol."'/>
										<input type='hidden' name='flcmeiopublichomol[]' id='flcmeiopublichomol_".$flcid."' value='".$flcmeiopublichomol."'/>
										<input type='hidden' name='flcordservnum[]' id='flcordservnum_".$flcid."' value='".$flcordservnum."'/>
										<input type='hidden' name='tflordem[]' id='flordem_".$flcid."' value='".$tflordem."'/>		
										<input type='hidden' name='tflid[]' id='tflid_".$flcid."' value='".$tflid."'/>
									</td>
								</tr>
							";
						}
					}
				?>
			</table>
		</td>
	</tr>
<?php 
	if ( $habilitado ):
?>	
	<tr bgcolor="#DEDEDE">
		<td colspan="3">
			<input type="button" name="botao" value="Editar Licitação" onclick="window.location='?modulo=principal/cadLicitacao&acao=E&licid=<?=$dados['licid']?>';"/>
			<input type="button" name="btn_declaracao" value="Declaração de Conformidade" onclick="abreDeclaracao()"/>
		</td>
	</tr>
<?php 
	endif;
?>	
</table>
<?php 
endif;
?>
<script type="text/javascript">
function abreDeclaracao(){
	return windowOpen( '?modulo=principal/cadDeclaracaoConformidade&acao=A','blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function visualizaFase(flcid){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=V&flcid='+flcid,'blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

DownloadArquivo = function(arqid){
	window.location = '?modulo=principal/exibeLicitacao&acao=A&requisicao=download&arqid='+arqid;
}
</script>