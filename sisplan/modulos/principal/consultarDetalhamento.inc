<?php
$where = array();
if($_POST['act'] == 'pesquisar' ){
    if($_POST['pplid'])
        $where[] = "dpl.pplid = {$_POST['pplid']}";
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
	$detalhePlanejamento	= new DetalhePlanejamento($_REQUEST['dplid']);
    $detalhePlanejamento->inativarRegistro($_REQUEST['dplid']);
    $db->commit();
    $db->sucesso('principal/consultarDetalhamento');
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
    <input type="hidden" id="dplid" name="dplid" value=""/>
    <input type="hidden" name="act" id="act" value="pesquisar" />
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr class="nome-descricao" style="<?= $style; ?>">
        <td align='right' class="SubTituloDireita" style="width:25%;">
             ID Planejamento:
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
                    $modelo = new DetalhePlanejamento();
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
                <input type="button" name="btnVoltar" value="Voltar" onclick="voltar();"/>
        </td>
    </tr>
</table>
</form>
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td>

    <?

                $arCabecalho = array("Ação",
                                     "ID Planejamento",
                                     "Unidade Executora",
                                     "Tipo de Projeto",
                                     "Prioridade",
                                     "Título",
                                     "Desafio(s) Estratégico(s)",
                                     "Situação",
                                     "Valor Custeio",
                                     "Valor Capital",
                                     "Total");

                    $acao = '<div style="text-align:center; width: 60px" nowrap="nowrap">';
                    $acao.= '<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar Detalhamento" onclick=\'location.href="?modulo=principal/incluirDetalhamento&acao=C&pplid={pplid}";\'> &nbsp';
                    $acao.= '<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Tramitação / Abrir Detalhamento" onclick=\'abrirDadosDetalhamento({dplid});\'> &nbsp';
                    $acao.= '<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir Detalhamento" onclick=\'remover({dplid});\'>';
                    $acao.= '</div>';

                $detalhePlanejamento = new DetalhePlanejamento();
                
                $rs = $detalhePlanejamento->listaByWhere($where, true);
                
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

	function abrirDadosDetalhamento( id )
	{
		window.open(
		'?modulo=principal/dadosDetalhamento&acao=A&dplid=' + id,
		'dadosdetalhamento',
		'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
		);
	}

	function pesquisar()
	{
		document.formulario.act.value = 'pesquisar';
		document.formulario.submit();
	}

	function limpar(){
		window.location = '?modulo=principal/consultarDetalhamento&acao=<?= $_REQUEST['acao']; ?>';
	}

	function voltar(){
		window.location = '?modulo=principal/consultarPlanejamento&acao=<?= $_REQUEST['acao']; ?>';
	}

	function remover( cod )
	{
		if ( confirm("Deseja excluir esse detalhamento?" ) ) {
			document.formulario.dplid.value = cod;
			document.formulario.act.value = 'remover';
			document.formulario.submit();
		}
	}
</script>