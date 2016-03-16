<?php
include APPRAIZ . "includes/workflow.php";

if ( $_REQUEST['requisicao'] ) {
    $arrRest = $_REQUEST['requisicao']( $_REQUEST );
    if ( $arrRest['msg'] ) {
        echo "<script>alert('{$arrRest['msg']}')</script>";
        echo "<script>window.location.href = 'fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}'</script>";
        exit;
    } else {
        header( "Location: fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}" );
    }
}

if ( $_REQUEST['download'] == 'S' ) {
    $arqid   = $_REQUEST['arqid'];
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file    = new FilesSimec();
    $arquivo = $file->getDownloadArquivo( $arqid );
    echo"<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

$sql = "SELECT os.docid,
			   os.tosid,
			   os.docidpf,
			   scs.scsid,
			   ans.ansid,
			   odsdetalhamento,
			   odssubtotalpf,
			   odsqtdpfdetalhada,
			   odsqtdpfestimada,
			   odsid,
               tosid,
               os.odsidpai,
			   to_char(odsdtprevinicio, 'dd/mm/YYYY') as odsdtprevinicio, to_char(odsdtprevtermino, 'dd/mm/YYYY') as odsdtprevtermino
		FROM fabrica.ordemservico os
		inner join fabrica.solicitacaoservico scs ON scs.scsid = os.scsid
		inner join fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
		WHERE odsid = {$_REQUEST['odsid']} ORDER BY odsid";

// buscando dados da OS
$oss               = $db->pegaLinha( $sql );
$exibeDivergencia  = false;
$exibeGlosa        = false;
$arrEstado         = wf_pegarEstadoAtual( $oss['docidpf'] );
$pfls              = arrayPerfil();

if ( $arrEstado['esdid'] == WF_ESTADO_CPF_AGUARDANDO_CONTAGEM 
        && (!in_array( PERFIL_GERENTE_PROJETO, $pfls ) || in_array(PERFIL_SUPER_USUARIO, $pfls) ) ) {
    $permissaoCampos = true;
    $ator            = "Empresa Item 2";
} else {
    $permissaoCampos = false;
    $ator            = "Fiscal Técnico";
}


$ordemServico             = new OrdemServico();
$historicoPrevisaoTermino = new HistoricoPrevisaoTermino();
$ordemServico             = $ordemServico->recuperePorId( $_REQUEST['odsid'] );

if ( $ordemServico->possuiGlosa() ) {
    $glosa = new Glosa();
    $glosa = $glosa->recupereGlosaPeloId( $ordemServico->getIdGlosa() );
}

$arrDadosSituacaoStatusOS = $historicoPrevisaoTermino->retornarDataPrevisaoTerminoInicialEmpresaItem2( $_REQUEST['odsid'] );

if ( strtoupper( $arrDadosSituacaoStatusOS['status_os'] ) == 'OS EM DIA' ) {
    $apresentaGlosa = false;
    $colorStatusOS  = 'green';
} else {
    $apresentaGlosa   = true;
    $colorStatusOS    = 'red';
    $qtdPFDetalhadaOS = $ordemServico->getQtdePfDetalhada();
    $percentualGlosa  = 0;

    if ( $arrDadosSituacaoStatusOS['dias_previstos'] > 0 ) {
        $valorIDP = ( ( $arrDadosSituacaoStatusOS['dias_em_atraso'] * 100) / $arrDadosSituacaoStatusOS['dias_previstos'] );
    } else {
        $valorIDP = 0;
    }

    if ( $valorIDP > 81 ) {
        $percentualGlosa = 20;
    } elseif ( $valorIDP >= 51 && $valorIDP <= 80 ) {
        $percentualGlosa = 15;
    } elseif ( $valorIDP >= 31 && $valorIDP <= 50 ) {
        $percentualGlosa = 10;
    } elseif ( $valorIDP >= 11 && $valorIDP <= 30 ) {
        $percentualGlosa = 5;
    }

    $qtdPFGlosa  = ($qtdPFDetalhadaOS * ( $percentualGlosa / 100 ));
    $textoPadrao = 'Demanda entregue fora do prazo';
    $sql         = " UPDATE fabrica.verificacaoos
                SET veoniveis = 'f', 
                    veoobs = 'Demanda entregue fora do prazo'
				 WHERE odsid = {$_REQUEST['odsid']}
				 ";
    $db->executar( $sql );
    $db->commit();
}

//abas
$menu[0]    = array( "descricao" => "Listar OS", "link"      => "fabrica.php?modulo=principal/listarOrdemServico&acao=A" );
$menu[1]    = array( "descricao" => "Dados OS de Origem", "link"      => "fabrica.php?modulo=principal/contagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}" );
$menu[2]    = array( "descricao" => "Contagem P.F.", "link"      => "fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}" );
echo montarAbasArray( $menu, "fabrica.php?modulo=principal/cadContagemOS&acao=A&odsid={$_REQUEST['odsid']}&scsid={$_REQUEST['scsid']}" );


$titulo = "Nº OS  <span style='cursor:pointer; color: #0066CC;' onclick=window.location.reload()>" . $oss['odsid'] . "</span>";
monta_titulo( $titulo, $ator );

$exibeGlosa = ( strtoupper( $arrDadosSituacaoStatusOS['status_os'] ) == 'OS EM ATRASO' && (!$ordemServico->isCancelada()) );
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link rel='stylesheet' type='text/css' href='./css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<style>
    .TituloTabela{
        background-color: #C5C5C5;font-weight:bold
    }
    .center{
        text-align:center
    }
    .link{
        cursor: pointer;
    }
    .middle{
        vertical-align: middle;
    }
</style>

<script type="text/javascript">
    function salvarOSPF()
    {
        if(validaFormulario()){
            if(document.getElementById('odssubtotalpf').value == ''){
                alert("Preencha o campo Subtotal de PF.");
                document.getElementById('odssubtotalpf').focus();
                return false;
            }
<?php if ( $oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA ): ?>
                if(document.getElementById('odsqtdpfdetalhada').value == ''){
                    alert("Preencha o campo Qtde P.F. Detalhada.");
                    document.getElementById('odsqtdpfdetalhada').focus();
                    return false;
                }
<?php else: ?>
                if(document.getElementById('odsqtdpfestimada').value == ''){
                    alert("Preencha o campo Qtde P.F. Estimada.");
                    document.getElementById('odsqtdpfestimada').focus();
                    return false;
                }
<?php endif; ?>
            $('#form_cad_contagem').submit();
        }
    }

    function validaFormulario(){
        if( jQuery.trim($('input[name="arquivo"]').val()) =='' || jQuery.trim( $('input[name="aosdsc_"]').val() ) ==''){
            alert('Favor informar todos os dados para anexar o arquivo!');
            return false;
        }else{
            return true;
        }
    }

    function excluirAnexo(arqid)
    {
        if(confirm("Deseja realmente excluir o anexo?")){
            $('[name=requisicao]').val("excluirAnexo");
            $('[name=arqid]').val(arqid);
            if($('[name=requisicao]').val() == "excluirAnexo" && $('[name=arqid]').val()){
                $('#form_cad_contagem').submit();
            }
        }
    }
</script>
<form name="form_cad_contagem" id="form_cad_contagem" method="post" action="" enctype="multipart/form-data">
    <input type="hidden" name="requisicao" value="salvarContagemPFOS" />
    <input type="hidden" name="odsid" value="<?php echo $_GET['odsid'] ?>" />
    <input type="hidden" name="arqid" value="" />
    <table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="95%" valign="top">
                <table class="tabela" width="100%" align="center" >
                    <tr>
                        <td width="25%"  class="SubTituloDireita">Status:</td>
                        <td style="color: <?php echo $colorStatusOS ?>; font-weight: bold">
                            <?php echo $arrDadosSituacaoStatusOS['status_os']; ?>
                        </td>
                    </tr>
                </table>
            </td>
            <td></td>
        </tr>
        <tr>
            <td valign="top" >
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td width="25%" class="SubTituloDireita">Número:</td>
                        <td><? echo $oss['odsid']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Disciplinas e Produtos:</td>
                        <td>
                            <?php $arrDisciplina = carregarDisciplinasProdutosPorAnalise( $oss['ansid'] ); ?>
                            <?php if ( is_array( $arrDisciplina ) ): ?>
                                <?php foreach ( $arrDisciplina as $disc ): ?>
                                    <?php
                                    $arrContratada[$disc['disciplina']]['dpedsc']     = $disc['disciplina'];
                                    $arrContratada[$disc['disciplina']]['produtos'][] = $disc['produto'];
                                    ?>
                                <?php endforeach; ?>
                            <?php endif; ?>
                            <?php if ( is_array( $arrContratada ) ): ?>
                                <?php $n = 1; ?>
                                <?php foreach ( $arrContratada as $d ): ?>
                                    <b><?php echo $n . ") " . $d['dpedsc'] ?></b><?php echo $d['produtos'][0] ? ": " . implode( ", ", $d['produtos'] ) . ";" : ";" ?><br />
                                    <?php $n++; ?>
                                <?php endforeach; ?>
                            <?php else: ?>
                                N/A
                            <?php endif; ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Previsão Início:</td>
                        <td><? echo $oss['odsdtprevinicio']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Prev. Término:</td>
                        <td>
                            <? echo $oss['odsdtprevtermino']; ?>
                            <?php if ( !empty( $_GET['odsid'] ) ): ?>
                                <script type="text/javascript">
                                    function alterarPrevisaoTermino(){

                                        window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&odsid=<?php echo $_GET['odsid']; ?>&alteracao=OSitem2', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                                    }
                                </script>
                                <a href="javascript: alterarPrevisaoTermino()"
                                   style="text-decoration: underline;">
                                    Redefinir Previs&atilde;o de T&eacute;rmino
                                </a>
                            <?php endif; ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Detalhamento:</td>
                        <td><? echo $oss['odsdetalhamento']; ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Subtotal de PF:</td>
                        <td>
                            <?php if ( $oss['odssubtotalpf'] ) $odssubtotalpf = number_format( $oss['odssubtotalpf'], 2, ",", "." ); ?>
                            <input type="hidden" name="odssubtotalpf_" value="<? echo $odssubtotalpf; ?>" />
                            <? echo campo_texto( 'odssubtotalpf', 'S', ($permissaoCampos ? "S" : "N" ), 'Subtotal de PF', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odssubtotalpf"' ); ?>
                        </td>
                    </tr>
                    <?php if ( $oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA ): ?>
                        <tr>
                            <td class="SubTituloDireita">Qtde P.F. Detalhada:</td>
                            <td>
                                <?php if ( $oss['odsqtdpfdetalhada'] ) $odsqtdpfdetalhada = number_format( $oss['odsqtdpfdetalhada'], 2, ",", "." ); ?>
                                <input type="hidden" name="odsqtdpfdetalhada_" value="<? echo $odsqtdpfdetalhada; ?>" />
                                <? echo campo_texto( 'odsqtdpfdetalhada', 'S', ($permissaoCampos ? "S" : "N" ), 'Qtde P.F. Detalhada', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfdetalhada"' ); ?>
                            </td>
                        </tr>
                    <?php else: ?>
                        <tr>
                            <td class="SubTituloDireita">Qtde P.F. Estimada:</td>
                            <td>
                                <?php if ( $oss['odsqtdpfestimada'] ) $odsqtdpfestimada = number_format( $oss['odsqtdpfestimada'], 2, ",", "." ); ?>
                                <input type="hidden" name="odsqtdpfestimada_" value="<? echo $odsqtdpfestimada; ?>" />
                                <? echo campo_texto( 'odsqtdpfestimada', 'S', ($permissaoCampos ? "S" : "N" ), 'Qtd. de P.F. Estimado', 15, 14, '###.###.###,##', '', '', '', 0, 'id="odsqtdpfestimada"' ); ?>
                            </td>
                        </tr>
                    <?php endif; ?>
                    <?php
                    if ( $apresentaGlosa && $ordemServico->passivelDeGlosaEmpresaItem2() && $ordemServico->possuiGlosa() ) {
                        //Só apresentará glosa quando tiver glosa no banco
                        ?>
                        <tr>
                            <td class="SubTituloDireita">Quantidade de ponto de função glosado:</td>
                            <td><?php echo $glosa->getValorEmPfComMascara(); ?></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Justificativa da glosa:</td>
                            <td><?php echo $glosa->getJustificativa(); ?></td>
                        </tr>
                    <?php 
                    }

						/*
						 * Criado por: Rondomar França
						 * SS831 - REQ004 - Detalhe da Ordem de Serviço(OS) - Finalizada
						 * */

						if( $arrEstado['esdid'] == WF_ESTADO_CPF_FINALIZADA || WF_ESTADO_OS_FINALIZADA == $arrEstado['esdid'] )
						{
							$sqlMemoLink = "select 
												a.memoid, b.memonumero || '/' || date_part('year', b.memodata) as memonumero_ano
											from fabrica.ordemservico a
											inner join fabrica.memorando b on a.memoid = b.memoid
											where a.odsid = " . $oss['odsid'];
							
							$rsMemoId = $db->pegaLinha( $sqlMemoLink );
							?>
							<tr>
								<td class="SubTituloDireita">Memorando:</td>
								<td><a href="fabrica.php?modulo=sistema/geral/memorando/visualizar&acao=A&memo=<?php echo($rsMemoId['memoid']); ?>"><?php echo( $rsMemoId['memonumero_ano'] );?></a></td>
						</tr>
						<?php
					}
					                    
					if ( $permissaoCampos ): ?>
                        <tr>
                            <td class="TituloTabela center" colspan="2">Anexar arquivos</td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Arquivo:</td>
                            <td><input type="file" name="arquivo" id="arquivo"></td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Tipo de anexo:</td>
                            <td>
                                <?
                                if ( $oss['tosid'] == TIPO_OS_CONTAGEM_DETALHADA ) {
                                    echo "Relatório de Contagem de PF Detalhada";
                                    echo "<input type='hidden' name='taoid' value='29'";
                                } elseif ( $oss['tosid'] == TIPO_OS_CONTAGEM_ESTIMADA ) {
                                    echo "Relatório de Contagem de PF Estimada";
                                    echo "<input type='hidden' name='taoid' value='28'";
                                } else {
                                    $sql = "SELECT taoid as codigo, taodsc as descricao FROM fabrica.tipoanexoordem WHERE taostatus='A' order by 2";
                                    $db->monta_combo( 'taoid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'taoid' );
                                }
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Descrição:</td>
                            <td><? echo campo_texto( 'aosdsc_', 'S', 'S', 'Descrição', 50, 255, '', '', '', '', 0, 'id="aosdsc_"' ); ?></td>
                        </tr>
                        <tr>
                            <td colspan="2" class="SubTituloDireita center">
                                <input type="button" name="btn_salvar" value="Salvar" onclick="salvarOSPF()" />
                                &nbsp;
                                <input type="button" name="btn_solicitacao" value="Voltar" onclick="history.back();" />
                            </td>
                        </tr>
                    <?php endif; ?>
                    <tr>
                        <td colspan="2" class="TituloTabela center">Anexos</td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <?php
                            $sql = "SELECT
									" . ($permissaoCampos ? "'<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer\" title=\"Excluir Anexo\" onclick=\"excluirAnexo(\'' || an.arqid || '\')\" /></center>' as acao," : "") . "
									to_char(an.aosdtinclusao,'dd/mm/YYYY HH24:MI'), tp.taodsc,
									'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" />' || ar.arqnome||'.'||ar.arqextensao ||'</a>',
									ar.arqtamanho, an.aosdsc
								FROM fabrica.anexoordemservico an
								LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
								LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
								WHERE odsid='" . $oss['odsid'] . "' AND aosstatus='A'
                                                                ORDER BY an.aosdtinclusao DESC";

                            if ( $permissaoCampos ) {
                                $cabecalho = array( "Ação", "Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição" );
                            } else {
                                $cabecalho = array( "Data inclusão", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição" );
                            }
                            $db->monta_lista_simples( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
                            ?>
                        </td>
                    </tr>
                </table>
            </td>
            <td valign="top" width="150" >
                <?php
                $dados_wf = array(
                    'odsid' => $oss['odsid'],
                    'tosid' => $oss['tosid']
                );
                
                
                wf_desenhaBarraNavegacao( $oss['docidpf'], $dados_wf );
                
                if( $exibeGlosa ) { ?>

                    <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                        <tr style="background-color: #c9c9c9; text-align: center;">
                            <td style="font-size: 7pt; text-align: center;">
                                <span title="estado atual">
                                    <b>outras ações</b>
                                </span>
                            </td>
                        </tr>
                        <tr>
                            <td id="inserirGlosa" style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9; cursor: pointer;"
                                onmouseover="this.style.backgroundColor='#ffffdd';"
                                onmouseout="this.style.backgroundColor='';">
                                <span style="color: #133368;">Glosa</span>
                            </td>
                        </tr>
                    </table>
                <?php } ?>
            </td>
        </tr>
    </table>
</form>

<div id="modalFormularioSalvarGlosa" style="display: none;">
    <form id="formularioSalvarGlosa" method="post" action="geral/cadastro_glosa_em_cadOsExecucao.php">
        <p class="paragrafo_glosa">
            <label style="display: block;">Qtde em dias de atraso:</label>
            <span class="campos"><?php echo $arrDadosSituacaoStatusOS['dias_em_atraso']; ?></span>
        </p>
        <p class="paragrafo_glosa">
            <label style="display: block;">Qtde em PF da glosa:</label>
            <span class="campos"><?php echo $qtdPFGlosa; ?></span>
        </p>
        <p class="paragrafo_glosa">
            <label style="display: block;">Data de inicio:</label>
            <span class="campos"><?php echo date_create( $arrDadosSituacaoStatusOS['odsdtprevinicio'] )->format( 'd/m/Y' ); ?></span>
        </p>
        <p class="paragrafo_glosa">
            <label style="display: block;">Data de término planejada:</label>
            <span class="campos"><?php echo $arrDadosSituacaoStatusOS['data_termino_planejada']; ?></span>
        </p>
        <p class="paragrafo_glosa">
            <label style="display: block;">Data de entrega:</label>
            <span class="campos"><?php echo $arrDadosSituacaoStatusOS['datatramitacao']; ?></span>
        </p>
        <p class="paragrafo_textarea">
            <label>Motivo da glosa</label>
            <?php
            if ( empty( $textoPadrao ) ) {
                $textoPadrao = $ordemServico->possuiGlosa() ? $glosa->getJustificativa() : "";
            }
            ?>
            <span class="campos"><?php echo $textoPadrao; ?></span>
        </p>
        <p>
            <?php if ( $ordemServico->possuiGlosa() ) { ?>
                <input type="hidden" name="glosaid" value="<?php echo $ordemServico->getIdGlosa() ?>" />
            <?php } ?>
            <input type="hidden" name="idOrdemServico" value="<?php echo $_REQUEST['odsid'] ?>"/>
            <input type="hidden" name="cpfUsuarioResponsavel" value="<?php echo $_SESSION['usucpf'] ?>"/>
            <input type="hidden" name="dataInclusao" value="<?php echo date( 'Y-m-d' ) ?>" />
        </p>
    </form>
</div>

<script type="text/javascript">
    
<?php if ( $arrRest['msg'] ): ?>
        alert('<?php echo $arrRest['msg'] ?>');
<?php endif; ?>
    
    function popPausarServico(){

        window.open('?modulo=principal/popup/pausarServico&acao=A&odsid=<?php echo $oss['odsid']; ?>', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

    }
</script>
