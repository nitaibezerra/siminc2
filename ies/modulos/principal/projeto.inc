<?php

 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Programador: Fernando Bagno (fernandobagno@gmail.com)
 */

$iesid = $_SESSION["ies"]["iesid"];
$pbiid = iesPegaProjeto( $iesid );

$iesHabilitado = iesPegarEstadoAtual( $pbiid ) == AGUARDANDO_VALIDACAO_CRITERIOS || $_REQUEST["acao"] == 'C' ? "disabled" : "";

switch ($_REQUEST["requisicao"]) {
	
	case "salvarprojeto":
		$ies->cadastraProjetoIes( $_REQUEST, $_FILES );
	break;
	
	case "downloadarquivo":
		$ies->downloadArquivo( $_REQUEST["arqid"] );
		exit;
	break;
	
	case "excluirarquivo":
		$ies->excluirArquivo( $_REQUEST["aprid"] );
	break;
	
}


//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";

print "<br/>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Projeto", "" );

$ies->cabecalhoIES( $iesid );

if( $pbiid ){
	
	$docid = iesCriarDocumento( $pbiid );
	$esdid = iesPegarEstadoAtual( $pbiid );
	
}

?>

<script language="JavaScript" src="./geral/js/ies.js"></script>

<form action="" method="post" name="formProjeto" id="formProjeto" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="salvarprojeto"/>
	<input type="hidden" name="iesid" id="iesid" value="<?=$iesid; ?>"/>
	<input type="hidden" name="pbiid" id="pbiid" value="<?=$pbiid; ?>"/>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">Arquivos do Projeto</td>
		</tr>
		<tr>
			<td width="100%" valign="top" style="background: none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<table class="" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
					<tr>
						<td class="subtitulodireita">Projeto</td>
						<td>
							<input type="file" name="projeto" id="projeto" <?=$iesHabilitado?>>
							<?php echo obrigatorio(); ?>
						</td>
					</tr>
					<tr>
						<td colspan="2" style="color: red;">
							Os arquivos devem estar, obrigatoriamente, no formato <b>PDF</b>.
						</td>
					</tr>
					<tr><td>&nbsp;</td></tr>
					<tr>
						<td colspan="2">
							<?php if ( $esdid != NAO_INICIADO ): ?>
								<a style="cursor: pointer;" onclick="abreProtocolo();">Clique aqui para visualizar o Nº de Protocolo.</a>
							<?php endif; ?>
						</td>
					</tr>
				</table>
			</td>
			<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<?php

					wf_desenhaBarraNavegacao( $docid , array( 'iesid' => $iesid, 'pbiid' => $pbiid, 'acao' => $_REQUEST["acao"] ) );
				
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<input type="button" value="Salvar" onclick="iesSalvarProjeto();" style="cursor: pointer;" <?=$iesHabilitado?>>
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;">
			</td>
		</tr>
	</table>
</form>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td colspan="2" class="subtitulocentro">Arquivos Anexados</td>
	</tr>
</table>
<?php
	if ( $pbiid ){
		$ies->montaListaProjetos( $pbiid, $_REQUEST["acao"] );	
	} 
?>