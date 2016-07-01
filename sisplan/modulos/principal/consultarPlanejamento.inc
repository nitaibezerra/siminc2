<?php
//Verificando a data de inicio e termino do planejamento ( definida na programacaoexercicio )
$programacaoExercicio = new ProgramacaoExercicio();
$permissao = $programacaoExercicio->verificaPermissaoPlanejamento();

$where = array();
if($_POST['act'] == 'pesquisar' ){
    if($_POST['pplid'])
        $where[] = "ppl.pplid = {$_POST['pplid']}";
    if($_POST['uexid'])
        $where[] = "uex.uexid = {$_POST['uexid']}";
    if($_POST['tppid'])
        $where[] = "tpp.tppid = {$_POST['tppid']}";
    if($_POST['esdid'])
        $where[] = "esd.esdid = {$_POST['esdid']}";
    if($_POST['pplprioridade'])
        $where[] = "ppl.pplprioridade = '{$_POST['pplprioridade']}'";
    if($_POST['ppltitulo'])
        $where[] = "ppl.ppltitulo ILIKE '%{$_POST['ppltitulo']}%'";

    extract($_POST);
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

$db->cria_aba($abacod_tela, $url, "");
monta_titulo( $titulo_modulo, '' );

if ( $_REQUEST['act'] == 'remover' ){
	$projetoPlanejamento	= new ProjetoPlanejamento($_REQUEST['pplid_excluir']);
	$dados 					= $projetoPlanejamento->getDados();

	$excecaoUnidade = new ExcecaoUnidade();
	$excecao		= $excecaoUnidade->verificaExcecao($dados['uexid']);

	if( $permissao || $excecao){
		$projetoPlanejamento->inativarRegistro($_REQUEST['pplid_excluir']);
		$db->commit();
		$db->sucesso('principal/consultarPlanejamento');
	}else{
		echo "<script> alert('Não foi possível excluir o registro, o planejamento não está disponível.'); </script>";
	}
}


// Verificando responsabilidades
$unidadeExecutora	= new UnidadeExecutora();
$responsabilidades	= $unidadeExecutora->getResponsabilidadeUnidadesByUsuario();

if( !$db->testa_superuser() &&
	!possuiPerfil(PERFIL_APOIO_COORDENADOR_ORCAMENTO) &&
	!possuiPerfil(PERFIL_APOIO_COORDENADOR_PLANEJAMENTO) &&
	!possuiPerfil(PERFIL_COORDENADOR_ORCAMENTO) &&
	!possuiPerfil(PERFIL_COORDENADOR_PLANEJAMENTO) ){
	if(is_array( $responsabilidades ) && !empty( $responsabilidades )){
		$cond 		= " uexid IN ('".implode("','", $responsabilidades)."') ";
		$condLista	= " uex.uexid IN ('".implode("','", $responsabilidades)."') ";
	}else{
		$cond = $condLista = " FALSE ";
	}
}else{
	$cond = $condLista = " TRUE ";
}

$where[] = $condLista;

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" name="formulario">
    <input type="hidden" id="pplid_excluir" name="pplid_excluir" value=""/>
    <input type="hidden" name="act" id="act" value="pesquisar" />
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr class="nome-descricao" style="<?= $style; ?>">
        <td align='right' class="SubTituloDireita" style="width:25%;">
             ID:
        </td>
        <td colspan="5">
             <?= campo_texto('pplid', 'N', 'S', '', 10, 4, '###########', '','','','','id="pplid"','','',''); ?>
        </td>
    </tr>
    <tr class="nome-descricao" style="<?= $style; ?>">
        <td align='right' class="SubTituloDireita" style="width:25%;">
             Prioridade:
        </td>
        <td colspan="5">
             <?= campo_texto('pplprioridade', 'N', 'S', '', 10, 4, '####', '','','','','id="pplprioridade"','','',''); ?>
        </td>
    </tr>
    <?php if( $_REQUEST['acao'] == 'A'){ ?>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                     Unidade Executora:
                </td>
                <td>
                    <?php
                        $modelo = new UnidadeExecutora();
                        $dados  = $modelo->recuperarTodos("uexid AS codigo, uexsigla || ' - ' || uexdsc AS descricao", array("uexstatus = 'A'",$cond), "descricao");
                        $db->monta_combo("uexid", $dados, 'S', '&nbsp', '', '', '', '517', 'N', 'uexid');
                    ?>
                </td>
            </tr>
    <?php } ?>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
             Tipo de Projeto:
        </td>
        <td>
            <?php
                $modelo = new TipoProjetoPlanejamento();
                $dados  = $modelo->recuperarTodos("tppid AS codigo, tppnome || ' - ' || tppdescricao AS descricao", array("tppstatus = 'A'"), "descricao");
                $db->monta_combo("tppid", $dados, 'S', '&nbsp', '', '', '', '517', 'N', 'tppid');
            ?>
        </td>
    </tr>
    <?php if( $_REQUEST['acao'] == 'A'){ ?>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Situação:
            </td>
            <td>
                <?php
                    $modelo = new ProjetoPlanejamento();
                    $dados  = $modelo->getComboSituacao();
                    $db->monta_combo("esdid", $dados, 'S', '&nbsp', '', '', '', '517', 'N', 'esdid');
                ?>
            </td>
        </tr>
    <?php } ?>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
             Título:
        </td>
        <td>
            <?= campo_textarea( 'ppltitulo', 'N', 'S', '', 95, 2, 300 ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align="right" style="width:25%;">
            &nbsp;
        </td>
        <td colspan="7">
                <input type="button" name="btnPesquisar" value="Pesquisar" onclick="pesquisar();"/>
                <input type="button" name="btnLimpar" value="Limpar" onclick="limpar();"/>
        </td>
    </tr>
    <?php if( $_REQUEST['acao'] == 'A'){ ?>
        <tr>
            <td colspan="2">
                <a style="cursor: pointer;" onclick="javascript:document.location='sisplan.php?modulo=principal/incluirPlanejamento&acao=<?= $_REQUEST['acao']; ?>';" title="Clique para incluir um novo Planejamento no sistema">
                    <img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir novo Planejamento</b>
                </a>
            </td>
        </tr>
    <?php } ?>
</table>
</form>
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td>

    <?

                $arCabecalho = array("Ação",
                                     "ID",
                                     "Unidade Executora",
                                     "Tipo de Projeto",
                                     "Prioridade",
                                     "Título",
                                     "Desafio(s) Estratégico(s)",
                                     "Situação",
                                     "Valor Custeio",
                                     "Valor Capital",
                                     "Total");

                if( $_REQUEST['acao'] == 'A'){
                    $acao = '<div style="text-align:center; width: 75px" nowrap="nowrap">';
                    $acao.= '<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick=\'location.href="?modulo=principal/incluirPlanejamento&acao=A&pplid={pplid}";\'> &nbsp';
                }else{
                    $acao = '<div style="text-align:center; width: 40px" nowrap="nowrap">';
                }
                $acao.= '<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Tramitação / Abrir Planejamento" onclick=\'abrirDadosPlanejamento({pplid});\'> &nbsp';
                if( $_REQUEST['acao'] == 'A'){
                    $acao.= '<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick=\'remover({pplid});\'> &nbsp';
                }
                $acao.= '<php>
                            if({esdid} == '.WF__PLANEJAMENTO_ESTADO_APROVADO.'){
                                return "<img src=\'/imagens/gif_inclui.gif\' style=\'cursor:pointer\' title=\'Detalhar\' onclick=\'detalhar({pplid});\'>";
                            }else{
                                return "<img src=\'/imagens/gif_inclui_d.gif\' title=\'Não é possível detalhar, o planejamento não se encontra aprovado.\' >";
                            }
                         </php>';
                $acao.= '</div>';

                $projetoPlanejamento = new ProjetoPlanejamento();
                
                if( $_REQUEST['acao'] == 'B'){
                    $where[] = " esd.esdid = '".WF__PLANEJAMENTO_ESTADO_APROVADO."' ";
                    $where[] = " dpl.dplid IS NULL ";
                }
                
                $rs = $projetoPlanejamento->listaByWhere($where, true);
                
                $arConfig = array("style" 			=> "width:100%;",
                                  "totalLinha" 		=> true,
                                  "totalRegistro" 	=> true);

                //Definindo todas as colunas de texto como string
                $arParamCol[0] = array("type" => Lista::TYPESTRING);
                $arParamCol[1] = array("type" => Lista::TYPESTRING);
                $arParamCol[2] = array("type" => Lista::TYPESTRING);
                $arParamCol[3] = array("type" => Lista::TYPESTRING);
                $arParamCol[4] = array("type" => Lista::TYPESTRING);
                $arParamCol[5] = array("type" => Lista::TYPESTRING);
                $arParamCol[6] = array("type" => Lista::TYPENUMERIC);
                $arParamCol[7] = array("type" => Lista::TYPENUMERIC);
                $arParamCol[8] = array("type" => Lista::TYPENUMERIC);

                $oLista = new Lista($arConfig);
                $oLista->setCabecalho( $arCabecalho );
                $oLista->setCorpo( $rs, $arParamCol );
                $oLista->setAcao( $acao );
                $oLista->show();
  ?>
        </td>
    </tr>
</table>

<script type="text/javascript">

	function abrirDadosPlanejamento( id )
	{
		window.open(
		'?modulo=principal/dadosPlanejamento&acao=A&pplid=' + id,
		'dadosplanejamento',
		'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function pesquisar()
	{
		document.formulario.act.value = 'pesquisar';
		document.formulario.submit();
	}

	function limpar(){
		window.location = '?modulo=principal/consultarPlanejamento&acao=<?= $_REQUEST['acao']; ?>';
	}

    function detalhar( pplid ){
		window.location = '?modulo=principal/incluirDetalhamento&acao=<?= $_REQUEST['acao']; ?>&pplid='+pplid;
	}

	function gerar()
	{
		document.formulario.act.value = 'xls';
		document.formulario.submit();
	}

	function remover( cod )
	{
		if ( confirm("Deseja excluir esse item?" ) ) {
			document.formulario.pplid_excluir.value = cod;
			document.formulario.act.value = 'remover';
			document.formulario.submit();
		}
	}
</script>