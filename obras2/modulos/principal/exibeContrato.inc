<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

////se não tiver obra retorna
//if ( empty($obrid) ){
//	die("<script>
//			alert('Faltam parametros para acessar esta tela!');
//	        history.go(-1);
//		 </script>");
//}

switch ( $_REQUEST['op'] ){
	case 'download':
		$arqid = $_GET['arqid'];
		if ( $arqid ){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$file 		  = new FilesSimec(null, null, "obras2");
			$file->getDownloadArquivo($arqid);
		}
		die();
}

require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$obra     = new Obras( $obrid );
$crtid    = $obra->pegaContratoPorObra( $obrid );
$contrato = new Contrato( $crtid );

$dados    = $contrato->getDados();

//dbg($dados, d);
if ( $dados['crtid'] ){
	$dados['crtdtassinatura']  = formata_data($dados['crtdtassinatura']);
	$dados['dt_cadastro']      = formata_data($dados['dt_cadastro']);
	$dados['crtdttermino'] 	   = formata_data($dados['crtdttermino']);
	$dados['crtvalorexecucao'] = number_format($dados['crtvalorexecucao'], 2, ',', '.');
	$dados['crtpercentualdbi'] = number_format($dados['crtpercentualdbi'], 2, ',', '.');
	
	extract( $dados );

	$obrvalorprevisto = number_format($obra->obrvalorprevisto, 2, ',', '.');
	
	$empresa 	= new Entidade( $entidempresa );        
	$entnomeempresa = "(". mascaraglobal($empresa->entnumcpfcnpj,"##.###.###/####-##") .") ".$empresa->entnome;

	$entidempresa 	= $empresa->getPrimaryKey();
}

$habilitaAditivo = false;
if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';
	
//	$arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );	
	$arMenuBlock = array();	
	
	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,$arMenuBlock);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,$arMenuBlock);
	}
	
//	$obra  = new Obras( $obrid );
	$esdid = pegaEstadoObra( $obra->docid );
	if ( $esdid == ESDID_OBJ_CONTRATACAO /*|| $esdid == ESDID_OBJ_ADITIVO*/ ){
		$habilitado = true;
		$habilita 	= 'S';
	}else{
		$habilitado = false;
		$habilita 	= 'N';
	}
	if ( $esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_ADITIVO ){
		$habilitaAditivo = true;
	}
}else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        
        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
        <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        
        
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
	$habilitado = false;
	$habilita 	= 'N';
	
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO, PFLCOD_GESTOR_MEC) ) ){
	$habilitado = false;
	$habilita = 'N';
}

$docid = pegaDocidObra( $obrid );

echo cabecalhoObra($obrid);
//echo '<br>';
monta_titulo( $titulo_modulo, '' );

if ( empty($dados['crtid']) ):
	$licitacao = new Licitacao();
	$licDados = $licitacao->pegaDadosPorObra( $obrid );
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="100%" class="subtitulocentro" style="color: red;" valign="top">
			<?php
			if ( count( $licDados ) ):
			?>
			Esta obra não está vinculada a nenhum contrato.
			<br>
<?php 
				if ( $habilitado ):
?>			
			<input type="button" name="botao" value="Cadastrar contrato para esta obra" onclick="window.location='?modulo=principal/cadContrato&acao=A';"/>
<?php 
				endif;
?>			
			<?php
			else:
			?>
			Para vincular esta obra a um contrato, primeiramente ela deve ser vinculada a uma licitação.
			<?php
			endif;
			?>
		</td>
		<td>
            <?php
            // Barra de estado WORKFLOW
            if( possui_perfil(PFLCOD_CALL_CENTER)){
                wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid), array('acoes' => true));
            }else if ( $docid ){
            	wf_desenhaBarraNavegacao($docid, array('obrid' =>  $obrid));
            }
            ?>		
		</td>
		
	</tr>
</table>
<?php 
else:
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" border="0">
	<tr>
            <td height="1">&nbsp;</td>
            <td>&nbsp;</td>
            <td rowspan="15" align="right" valign="top" width="1">
            <?php
                // Barra de estado WORKFLOW
            if( possui_perfil(PFLCOD_CALL_CENTER)){
                wf_desenhaBarraNavegacao( $docid, array('obrid' => $obrid), array('acoes' => true) );
            }else if($docid){
           	wf_desenhaBarraNavegacao($docid, array('obrid' =>  $obrid));
            }    
                // Excluir Aditivos da Obra
                if ($ttaid && possuiPerfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC, PFLCOD_SUPERVISOR_UNIDADE))):
                    wf_botao_excluir_aditivo($obrid);
                endif;
            ?>
            </td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Licitação:</td>
		<td>
			<?php 
				$licitacao = new Licitacao();
				$dadoLic   = $licitacao->pegaDadosPorObra( $obrid );
				echo $dadoLic['licnumero'] . ' - ' . $dadoLic['moldsc']; 
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Empresa Contratada</td>
		<td>
		  <span id="entnomeempresa"><?php echo $entnomeempresa; ?></span>
		  <input type="hidden" name="entidempresa" id="entidempresa" value="<?php echo $entidempresa; ?>">
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data de Assinatura do Contrato:</td>
		<td><?=$crtdtassinatura;?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
		<td><?=$crtprazovigencia?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data de término do contrato:</td>
		<td><?=$crtdttermino?> <span style="background: #f00; color: #fff">Atenção: Após esta data não será possível repassar recursos para esta obra. Mantenha a validade do contrato através de aditivos de prazo.</span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do Contrato (R$):</td>
		<td><?=$crtvalorexecucao?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor Previsto da Obra (R$):</td>
		<td><?=$obrvalorprevisto;?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual BDI:</td>
		<td><?=$crtpercentualdbi?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
	</tr>
    <tr>
		<td class="SubTituloDireita">Data de Inserção:</td>
		<td><?=$dt_cadastro?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"> Contrato digitalizado em PDF:</td>
		<td>
			<?php
			$arquivo = new Arquivo( $arqidcontrato );
			if ( $arquivo->arqid ){
				echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
						<img src='/imagens/salvar.png' border='0'>
						" . $arquivo->arqnome . "." . $arquivo->arqextensao . "
					  </a>";
			}
			?>
		</td>
	</tr>
    <?
        if(obraMi($obrid)):
            $itenscomposicao = new ItensComposicaoObras();
    ?>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das Etapas do Cronograma da Edificação (R$):</td>
                <td>
                    <?= number_format( $itenscomposicao->getValorTotalItens($obrid, 'D'), 2, ',', '.'); ?>
                </td>
            </tr>

            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das Etapas dos Serviços Externos (R$): </td>
                <td>
                    <?= number_format( $itenscomposicao->getValorTotalItens($obrid, 'F'), 2, ',', '.'); ?>
                </td>
            </tr>
    <? endif; ?>
<?php
if ( $ttaid ):
?>
	<tr>
		<td width="265" class="subtitulodireita">&nbsp;</td>
		<td>
<?php 
//	if ( $habilitado ):
?>		
                    Esta obra foi aditivada, para visualizar o histórico de aditivos
                    <a href="javascript:janela('?modulo=principal/historicoAditivo&acao=E&crtid=<?php echo $crtid; ?>',600,800,'historicoAditivo')">clique aqui.</a>
                    
<?php 
//	endif;
?>		
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
		<td>
			<?php 
			$tipoTermo = new TipoTermoAditivo( $ttaid );
			echo $tipoTermo->ttadsc;
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Denominação:</td>
		<td>
			<?php 
			echo $crtdenominacao;
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
		<td>
			<?php 
			echo formata_data( $crtdtassinaturaaditivo );
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Aditivo de Supressão:</td>
		<td>
			<?php 
			echo ($crtsupressao == 't' ? 'Sim' : 'Não');
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Justificativa:</td>
		<td>
			<?php 
			echo nl2br( $crtjustificativa );
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Anexo Assinado digitalizado em PDF:</td>
		<td>
			<?php
			$arquivo = new Arquivo( $arqid );
			if ( $arquivo->arqid ){
				echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
						<img src='/imagens/salvar.png' border='0'>
						" . $arquivo->arqnome . "." . $arquivo->arqextensao . "
					  </a>";
			}
			?>
		</td>
	</tr>	
<?php 
endif;
?>
	
	<? if( !obraMi($obrid) ): ?>

	<tr bgcolor="#FFFFFF" id="tr_obra_contrato" style="display: <?=$licid ? '' : 'none' ?>;">
            <td colspan="3" valign="top" id="td_obra_contrato">
                <div style="margin-bottom: 7px;">
                        Vínculo da obra no contrato
                </div>
                <?php
                if ( $crtid ):
                        $obraContrato = new ObrasContrato();
                        $dadosObraContrato = $obraContrato->listaByContrato( $crtid, $obrid );
                        foreach ( $dadosObraContrato as $dados ):
                                $umdid 			  		= $dados['umdid']; 
                                $ocrqtdconstrucao     = $dados['ocrqtdconstrucao']; 
                                $ocrdtordemservico    = formata_data( $dados['ocrdtordemservico'] ); 
                                $ocrdtinicioexecucao  = formata_data( $dados['ocrdtinicioexecucao'] ); 
                                $ocrprazoexecucao     = $dados['ocrprazoexecucao']; 
                                $ocrdtterminoexecucao = formata_data(  $dados['ocrdtterminoexecucao'] ); 
                                $ocrvalorexecucao     = number_format( $dados['ocrvalorexecucao'], 2, ',', '.'); 
                                $ocrcustounitario     = number_format( $dados['ocrcustounitario'], 2, ',', '.'); 
                                $ocrpercentualdbi     = number_format( $dados['ocrpercentualdbi'], 2, ',', '.'); 
                                $obrid                = $dados['obrid']; 
                                $obrnome              = $dados['obrnome']; 
                                $arqidos              = $dados['arqidos']; 
                                $arqidcusto           = $dados['arqidcusto']; 
                ?>
                <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" id="table_obra_<?php echo $obrid ?>" style="margin-bottom: 10px;">
                        <tr>
                                <td class="subtitulodireita" width="20%">
                                        Obra:
                                        <input name="obrid[]" id="obrid" value="<?php echo $obrid ?>" type="hidden">
                                </td>
                                <td id="txt_obra" width="35%" colspan="3">
                                        <?php echo $obrid . ' - ' . $obrnome ?>
                                </td>
                        </tr>
                        <tr>
                                <td class="subtitulodireita">Data de assinatura da ordem de serviço:</td>
                                <td><?=$ocrdtordemservico?></td>
                                <td class="SubTituloDireita">Total da planilha contratada (R$):</td>
                                <td><?=$ocrvalorexecucao?></td>
                        </tr>
                        <tr>
                                <td class="subtitulodireita">Início da Execução:</td>
                                <td><?=$ocrdtinicioexecucao?></td>
                                <td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
                                <td>
                                        <?php
                                        $unidadeMedida = new UnidadeMedida( $umdid );
                                        echo $ocrqtdconstrucao; 
                                        echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $unidadeMedida->umdeesc;
                                        ?>
                                </td>
                        </tr>
                        <tr>
                                <td class="subtitulodireita"></td>
                                <td></td>
                                <td class="SubTituloDireita">Custo Unitário R$:</td>
                                <td><?=$ocrcustounitario?> (R$ / Unidade de Medida)</td>
                        </tr>
                        <tr>
                                <td class="SubTituloDireita"></td>
                                <td></td>
                                <td class="SubTituloDireita">Percentual BDI:</td>
                                <td><?=$ocrpercentualdbi?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
                        </tr>
                        <tr>
                                <td align='right' class="SubTituloDireita" style="vertical-align:top;">Ordem de Serviço assinada e em PDF:</td>
                                <td>
                                        <?php
                                        if ( $obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C ){
                                                $ordemServico = new OrdemServicoMI();
                                                $ordemServico->carregarPorObrid( $obrid );

                                                $arquivo = new Arquivo( $ordemServico->arqid );
                                                if ( $arquivo->arqid ){
                                                        echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
                                                                        <img src='/imagens/salvar.png' border='0'>
                                                                        " . $arquivo->arqnome . "." . $arquivo->arqextensao . "
                                                                  </a>";
                                                }
                                        }else{
                                                $arquivo = new Arquivo( $arqidos );
                                                if ( $arquivo->arqid ){
                                                        echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
                                                                        <img src='/imagens/salvar.png' border='0'>
                                                                        " . $arquivo->arqnome . "." . $arquivo->arqextensao . "
                                                                  </a>";
                                                }
                                        }
                                        ?>
                                </td>
                                <td align='right' class="SubTituloDireita" style="vertical-align:top;">Planilha de Custo Contratada assinada e em PDF:</td>
                                <td>
                                        <?php
                                        $arquivo = new Arquivo( $arqidcusto );
                                        if ( $arquivo->arqid ){
                                                echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
                                                                <img src='/imagens/salvar.png' border='0'>
                                                                " . $arquivo->arqnome . "." . $arquivo->arqextensao . "
                                                          </a>";
                                        }
                                        ?>
                                </td>
                        </tr>

                </table>				
                <?php
                        endforeach;
                endif;
                unset( $umdid, $ocrqtdconstrucao, $ocrdtordemservico, $ocrdtinicioexecucao, $ocrprazoexecucao, $ocrdtterminoexecucao, $ocrvalorexecucao, $ocrcustounitario, $ocrpercentualdbi, $obrid, $obrnome );
                ?>				
            </td>
	</tr>
    <? endif; ?>
	<tr bgcolor="#DEDEDE">
		<td colspan="3">
<?php 
	if ( $habilitado ):	
?>
			<input type="button" name="botao" value="Editar Contrato" onclick="window.location='?modulo=principal/cadContrato&acao=E&crtid=<?=$crtid?>';"/>
<?php 
	endif;	
?>
<?php 

    // Forçado para false por essa função foi substituida pela barra de ferramentas
	if ( false ):	
?>
			<input type="button" name="botao" value="Inserir Aditivo" onclick="window.location='?modulo=principal/cadContrato&acao=E&crtid=<?=$crtid?>&op=liberaAditivo';"/>
<?php 
	endif;	
?>
		</td>
	</tr>        
        
</table>	
<?php 
endif;
?>

<?php
    $arrPerfisVisualizacao = array(PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_SUPER_USUARIO, PFLCOD_EMPRESA_VISTORIADORA_GESTOR, PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_GESTOR_MEC);
    if( ($obra->tpoid == 104 || $obra->tpoid == 105) && possuiPerfil($arrPerfisVisualizacao)){
        $_SESSION['esconde_btn_fechar_os_mi'] = true;
        ?>
        <tr>
            <td colspan="3">
                <script type="text/javascript">
                    jQuery(document).ready(function(){
                        jQuery.ajax({
                              url: "?modulo=principal/espelhoOsMi&acao=A",
                              cache: false
                           })
                          .done(function( html ) {
                            jQuery("#espelho_os_mi").append( html );
                          });
                    });
                </script>                    
                <div style="width: 100%; height: 100%;" id="espelho_os_mi">

                </div>
            </td>
        </tr>
        <?php
    }        
?>