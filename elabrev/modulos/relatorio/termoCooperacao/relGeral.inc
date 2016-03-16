<?php
/**
 * Relatório geral do Módulo Programação Orçamentária.
 */
header('content-type: text/html; charset=ISO-8859-1');

$hrefRedir = 'elabrev.php?modulo=relatorio/termoCooperacao/relGeral&acao=A';
// transforma consulta em pública
if ($_REQUEST['prtid'] && $_REQUEST['publico']) {
    $sql = sprintf(
            "UPDATE public.parametros_tela
               SET prtpublico = CASE WHEN prtpublico = true THEN false ELSE true END
               WHERE prtid = %d",
            $_REQUEST['prtid']
    );
    $db->executar($sql);
    $db->commit();
?>
<script type="text/javascript">
    location.href = '<?php echo $hrefRedir; ?>';
</script>
    <?
    die;
}

// remove consulta
if ($_REQUEST['prtid'] && $_REQUEST['excluir'] == 1) {
    $sql = sprintf(
        "DELETE from public.parametros_tela WHERE prtid = %d",
        $_REQUEST['prtid']
    );
    $db->executar($sql);
    $db->commit();
    ?>
<script type="text/javascript">
    location.href = '<?php echo $hrefRedir; ?>';
</script>
    <?
    die;
}
// executa e exibe a consulta solicitada pelo usuário
if (isset($_REQUEST['form']) == true && !$_REQUEST['carregar']) {
    if ($_REQUEST['prtid']) {
        $sql = sprintf("select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid']);
        $itens = $db->pegaUm($sql);
        $dados = unserialize(stripslashes(stripslashes($itens)));
        $_REQUEST = $dados;
        unset($_REQUEST['salvar']);
    }
    switch ($_REQUEST['pesquisa']) {
        case '1':
            require_once("os_resultado.inc");
            exit;
        case '2':
            include "osxls_resultado.inc";
            exit;
    }
}

// remove flag de submissão de formulário
if ($_REQUEST['prtid'] && $_REQUEST['carregar']) {
    $carregarAux = $_REQUEST['carregar'];
    unset($_REQUEST['form']);
}

// -- Atalho para recuperar os labels dos agrupadores e colunas
$arLabels = retornaColunasELabels();

// carrega as configurações de uma consulta armazenada anteriormente
if ($_REQUEST['prtid'] && $_REQUEST['carregar'] == 1) {
    $sql = sprintf("select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid']);
    $itens = $db->pegaUm($sql);
    $dados = unserialize(stripslashes(stripslashes($itens)));
    extract($dados);
    $_REQUEST = $dados;
    unset($_REQUEST['form']);
    unset($_REQUEST['pesquisa']);
    $titulo = $_REQUEST['titulo'];
    $chkpause = $_REQUEST['chkpause'];
    $demandaematraso = $_REQUEST['demandaematraso'];
    $tiporel = $_REQUEST['tiporel'];

    $agrupadorAux = array();
    $colunaAux = array();
    $pesquisacampoAux = array();
    $ordenarcampoAux = array();


    if ($_REQUEST['agrupador']) { // -- Agrupadores da consulta salva
        foreach ($_REQUEST['agrupador'] as $cod) {
            array_push($agrupadorAux, array('codigo' => $cod, 'descricao' => $arLabels[$cod]));
        }
    }

    if ($_REQUEST['coluna']) { // -- Colunas da consulta salva
        foreach ($_REQUEST['coluna'] as $cod) {
            array_push($colunaAux, array('codigo' => $cod, 'descricao' => $arLabels[$cod]));
        }
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório Geral";
monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');
?>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">
<!--
function exibeRelatorioGeral(tipo) {

    var formulario = document.formulario;
    var agrupador = document.getElementById('agrupador');
    var coluna = document.getElementById('coluna');
    var ordenarcampo = document.getElementById('ordenarcampo');

    // Tipo de relatorio
    formulario.pesquisa.value = '1';
    prepara_formulario();
    selectAllOptions( formulario.agrupador );
    selectAllOptions( coluna );
    selectAllOptions( ordenarcampo );

    if (tipo == 'relatorio') {
        formulario.action = '<?php echo $hrefRedir; ?>';
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
    } else {
        if (tipo == 'exibir') {
            if (!agrupador.options.length) {
                alert('Favor selecionar ao menos um item para agrupar o resultado!');
                return false;
            }
            if (!coluna.options.length) {
                alert('Favor selecionar ao menos uma coluna!');
                return false;
            }
            selectAllOptions(agrupador);
            selectAllOptions(coluna);
            selectAllOptions(ordenarcampo);

            formulario.target = 'resultadoGeral';
            var janela = window.open('', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        } else if(tipo == 'salvar') {
            if (formulario.titulo.value == '') {
                alert( 'É necessário informar o título do relatório!' );
                formulario.titulo.focus();
                return;
            }
            var nomesExistentes = new Array();
            <?php
            $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
            $nomesExistentes = $db->carregar( $sqlNomesConsulta );
            if ($nomesExistentes){
                foreach ($nomesExistentes as $linhaNome) {
                    print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
                }
            }
            ?>
            var confirma = true;
            var i, j = nomesExistentes.length;
            for (i = 0; i < j; i++) {
                if (nomesExistentes[i] == formulario.titulo.value) {
                    confirma = confirm('Deseja alterar a consulta já existente?');
                    break;
                }
            }
            if (!confirma) {
                return;
            }
            formulario.target = '_self';
            formulario.action = 'elabrev.php?modulo=relatorio/termoCooperacao/relGeral&acao=A&salvar=1';
        }
    }
    formulario.submit();
}


function exibeRelatorioGeralXLS(){
    var formulario = document.formulario;
    var agrupador = document.getElementById('agrupador');
    var coluna = document.getElementById('coluna');
    var ordenarcampo = document.getElementById('ordenarcampo');

    // Tipo de relatorio
    formulario.pesquisa.value = '2';

    prepara_formulario();
    selectAllOptions(formulario.agrupador);

    if (!agrupador.options.length) {
        alert('Favor selecionar ao menos um item para agrupar o resultado!');
        return false;
    }

    if (!coluna.options.length){
        alert('Favor selecionar ao menos uma coluna!');
        return false;
    }

    selectAllOptions(agrupador);
    selectAllOptions(coluna);
    selectAllOptions(ordenarcampo);
    formulario.submit();
}
	function tornar_publico( prtid ){
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function excluir_relatorio( prtid ){
		if(confirm('Deseja realmente excluir este relatório?')){
			document.formulario.excluir.value = '1';
			document.formulario.prtid.value = prtid;
			document.formulario.target = '_self';
			document.formulario.submit();
		}
	}
	
	function carregar_consulta( prtid ){
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_relatorio( prtid ){
		document.formulario.prtid.value = prtid;
		exibeRelatorioGeral( 'relatorio' );
	}

	
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}

	/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	function criarrequisicao() {
		return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
	}
	/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
	
	/* FUNÇÃO QUE TRATA O RETORNO */
	var pegarretorno = function () {
		try {
				if ( evXmlHttp.readyState == 4 ) {
					if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
						// criando options
						var x = evXmlHttp.responseText.split("&&");
						for(i=1;i<(x.length-1);i++) {
							var dados = x[i].split("##");
							document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
						}
						var dados = x[0].split("##");
						document.getElementById('usrs').options[0] = new Option(dados[1],dados[0]);
						document.getElementById('usrs').value = cpfselecionado;
					}
					if ( evXmlHttp.dispose ) {
						evXmlHttp.dispose();
					}
					evXmlHttp = null;
				}
			}
		catch(e) {}
	};
	/* FIM - FUNÇÃO QUE TRATA O RETORNO */
			
				
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	
//-->
</script>

<form action="" method="post" name="formulario" id="filtro"> 
    <input type="hidden" name="form" value="1"/>
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
    <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
    <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita">Título</td>
            <td>
                <?= campo_texto( 'titulo', 'N', $somenteLeitura, '', 65, 100, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="15%">Agrupadores:</td>
            <td>
            <?php
            // Início dos agrupadores
            $agrupador = new Agrupador('filtro','');

            // Dados padrão de destino (nulo)
            $destino = array();
            $destino = $agrupadorAux;
            $origem = array(
                array('codigo' => 'unidadegestorap', 'descricao' => $arLabels['unidadegestorap']),
                array('codigo' => 'unidadegestorac', 'descricao' => $arLabels['unidadegestorac']),
                array('codigo' => 'esddsc', 'descricao' => $arLabels['esddsc']),
                array('codigo' => 'coodsc', 'descricao' => $arLabels['coodsc']),
                array('codigo' => 'valor', 'descricao' => $arLabels['valor']),
                array('codigo' => 'ptres_desc', 'descricao' => $arLabels['ptres_desc']),
                array('codigo' => 'plicod', 'descricao' => $arLabels['plicod']),
                array('codigo' => 'ntddsc', 'descricao' => $arLabels['ntddsc']),
            );

            // exibe agrupador
            $agrupador->setOrigem( 'naoAgrupador', null, $origem );
            $agrupador->setDestino( 'agrupador', null, $destino );
            $agrupador->exibir();
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="15%">Colunas:</td>
            <td>
            <?php
            unset($agrupador, $destino, $origem);
            // Início dos agrupadores
            $agrupador = new Agrupador('filtro','');

            // Dados padrão de destino (nulo)
            $destino = array();
            $destino = $colunaAux;
            $origem = array(
                array('codigo' => 'unidadegestorap', 'descricao' => $arLabels['unidadegestorap']),
                array('codigo' => 'unidadegestorac', 'descricao' => $arLabels['unidadegestorac']),
                array('codigo' => 'esddsc', 'descricao' => $arLabels['esddsc']),
                array('codigo' => 'coodsc', 'descricao' => $arLabels['coodsc']),
                array('codigo' => 'valor', 'descricao' => $arLabels['valor']),
                array('codigo' => 'ptres_desc', 'descricao' => $arLabels['ptres_desc']),
                array('codigo' => 'plicod', 'descricao' => $arLabels['plicod']),
                array('codigo' => 'ntddsc', 'descricao' => $arLabels['ntddsc']),
            );

            // exibe coluna
            $agrupador->setOrigem('naoColuna', null, $origem);
            $agrupador->setDestino('coluna', null, $destino);
            $agrupador->exibir();
            ?>
            </td>
        </tr>
    </table>			
    <!-- OUTROS FILTROS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco( 'outros' );">
                <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                Relatórios Gerenciais
                <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="outros_div_filtros_off"></div>
	
    <div id="outros_div_filtros_on" style="display:none;">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
            <tr>
                <td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
                <?php
                $sql = sprintf(
                        "SELECT CASE WHEN prtpublico = true AND usucpf = '%s' THEN 
                                        '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
                                         <img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
                                         <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                     ELSE 
                                        '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ');\">' 
                                     END as acao, 
                                '<a title=\" Carregar consulta no formulário \" href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao 
                           FROM public.parametros_tela 
                           WHERE mnuid = %d AND prtpublico = TRUE",
                        $_SESSION['usucpf'],
                        $_SESSION['mnuid'],
                        $_SESSION['usucpf']
                );
                $cabecalho = array('Ação', 'Nome');
                ?>
                <td><?php $db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null, null, array(20, 80), 80); ?>
                </td>
            </tr>
        </table>
    </div>
    <script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

    <!-- FIM OUTROS FILTROS -->
    <!-- MINHAS CONSULTAS -->

    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco( 'minhasconsultas' );">
                <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                Minhas Consultas
                <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="minhasconsultas_div_filtros_off"></div>
    <div id="minhasconsultas_div_filtros_on" style="display:none;">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
            <tr>
                <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                <?php
                $sql = sprintf(
                        "SELECT CASE WHEN prtpublico = false THEN 
                                                 CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN
                                                        '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                                         <img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                         <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                 ELSE
                                                        '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">'
                                                 END 
                                         ELSE 
                                                 CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN
                                                        '<img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                         <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                 ELSE
                                                        '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">'
                                                 END 
                                 END as acao, 
                                --'<div id=\"nome_' || prtid || '\">' || prtdsc || '</div>' as descricao
                                '<a title=\" Carregar consulta no formulário \" href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao 
                         FROM 
                                public.parametros_tela 
                         WHERE 
                                mnuid = %d AND usucpf = '%s'",
                        $_SESSION['mnuid'],
                        $_SESSION['usucpf']
                );
                $cabecalho = array('Ação', 'Nome');
                ?>
                <td><?php $db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null, null, array(20, 80), 80); ?></td>
            </tr>
        </table>
    </div>
    <!-- FIM MINHAS CONSULTAS -->
    <!-- FILTROS -->
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita">Ano de Referência:</td>
            <td>
                <?php
                $arAnoReferencia = array();
                for ($y = -10; $y <= 10; $y++) {
                    $arAnoReferencia[] = array(
                        'codigo' => date('Y') + $y,
                        'descricao' => date('Y') + $y
                    );
                }
                $db->monta_combo(
                        'proanoreferencia',
                        $arAnoReferencia,
                        'S',
                        'Selecione...',
                        '',
                        '',
                        '',
                        '',
                        'N',
                        'proanoreferencia',
                        '',
                        isset($_REQUEST['proanoreferencia'])
                            ?$_REQUEST['proanoreferencia']
                            :date('Y')
                );
                ?>
            </td>
        </tr>
        <?php
        // -- Natureza da despesa
        $naturezadespesa = array();
        if ($_REQUEST['naturezadespesa'] && $_REQUEST['naturezadespesa'][0] != '') {
            array_walk($_REQUEST['naturezadespesa'], 'quote');
            $sql_carregados = "SELECT DISTINCT ndpid AS codigo,
                                      SUBSTR(ndpcod, 1, 6) || ' - ' || ndpdsc AS descricao
                                 FROM public.naturezadespesa
                                 WHERE ndpstatus = 'A'
                                   AND sbecod = '00'
                                   AND edpcod != '00'
                                   AND SUBSTR(ndpcod, 1, 2) NOT IN('31', '32', '46')
                                   AND (SUBSTR(ndpcod, 3, 2) IN('80', '90', '91') OR SUBSTR(ndpcod, 1, 6) IN ('335039'))
                                   AND ndpid IN(" . implode(',', $_REQUEST['naturezadespesa']) . ")
                                 ORDER BY 2";
            $naturezadespesa = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT DISTINCT ndpid AS codigo,
                         SUBSTR(ndpcod, 1, 6) || ' - ' || ndpdsc AS descricao
                    FROM public.naturezadespesa
                    WHERE ndpstatus = 'A'
                      AND sbecod = '00'
                      AND edpcod != '00'
                      AND SUBSTR(ndpcod, 1, 2) NOT IN('31', '32', '46')
                      AND (SUBSTR(ndpcod, 3, 2) IN('80', '90', '91') OR SUBSTR(ndpcod, 1, 6) IN ('335039'))
                    ORDER BY 2";
        mostrarComboPopup('Natureza da Despesa:', 'naturezadespesa', $stSql, '', 'Selecione a(s) Natureza(s) da Despesa(s)');

        // -- Plano interno
        $planointerno = array();
        if ($_REQUEST['planointerno'] && $_REQUEST['planointerno'][0] != '') {
            array_walk($_REQUEST['planointerno'], 'quote');
            $sql_carregados = "SELECT p.pliid AS codigo,
                                      plicod || ' - ' || plidsc AS descricao
                                 FROM monitora.pi_planointerno p
                                 INNER JOIN monitora.pi_planointernoptres pt USING(pliid)
                                 WHERE ndpid IN(" . implode(',', $_REQUEST['planointerno']) . ")
                                 ORDER by 2";
            $planointerno = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT p.pliid AS codigo,
                         plicod || ' - ' || plidsc AS descricao
                    FROM monitora.pi_planointerno p
                    INNER JOIN monitora.pi_planointernoptres pt USING(pliid)
                    ORDER by 2";
        mostrarComboPopup('Plano Interno:', 'planointerno', $stSql, '', 'Selecione o(s) Plano(s) Interno(s)');

        // -- Planos de trabalho
        $planotrabalho = array();
        if ($_REQUEST['planotrabalho'] && $_REQUEST['planotrabalho'][0] != '') {
            array_walk($_REQUEST['planotrabalho'], 'quote');
            $sql_carregados = "SELECT DISTINCT p.ptrid as codigo,
                                      ptres || ' - '
                                        || p.funcod || '.'
                                        || p.sfucod || '.'
                                        || p.prgcod || '.'
                                        || p.acacod || '.'
                                        || p.unicod || '.'
                                        || p.loccod AS descricao
                                    FROM monitora.ptres p
                                    JOIN public.unidadegestora u ON u.unicod = p.unicod
                                    WHERE p.ptrano = '{$_SESSION['exercicio']}'
                                      AND p.ptrid IN (" . implode(',', $_REQUEST['planotrabalho']) . ")
                                      AND p.ptrstatus = 'A'
                                      AND u.unicod IN ('26101', '26298', '26291', '26290')";
            $planotrabalho = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT DISTINCT p.ptrid as codigo,
                                  ptres || ' - '
                                    || p.funcod || '.'
                                    || p.sfucod || '.'
                                    || p.prgcod || '.'
                                    || p.acacod || '.'
                                    || p.unicod || '.'
                                    || p.loccod AS descricao
                    FROM monitora.ptres p
                    JOIN public.unidadegestora u ON u.unicod = p.unicod
                    WHERE p.ptrano = '{$_SESSION['exercicio']}'
                      AND p.ptrstatus = 'A'
                      AND u.unicod IN('26101', '26298', '26291', '26290')";
        mostrarComboPopup('Plano de Trabalho:', 'planotrabalho', $stSql, '', 'Selecione o(s) Plano(s) de Trabalho');

// -- Unidade gestora proponente
        $unidadegestorap = array();
        if ($_REQUEST['unidadegestorap'] && $_REQUEST['unidadegestorap'][0] != '') {
            array_walk($_REQUEST['unidadegestorap'], 'quote');
            $sql_carregados = "SELECT ung.ungcod AS codigo,
                                      ung.ungcod || ' - ' || ung.ungdsc AS descricao
                                 FROM public.unidadegestora ung
                                 WHERE ung.ungcod IN(" . implode(',', $_REQUEST['unidadegestorap']) .")
                                 ORDER BY ung.ungabrev";
            $unidadegestorap = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT ung.ungcod AS codigo,
                         ung.ungcod || ' - ' || ung.ungdsc AS descricao
                    FROM public.unidadegestora ung
                    ORDER BY ung.ungabrev";
        mostrarComboPopup('Unidade Gestora do Proponente:', 'unidadegestorap', $stSql, '', 'Selecione a(s) Unidades(s) Gestora(s)');

        // -- Unidade gestora cedente
        $unidadegestorac = array();
        if ($_REQUEST['unidadegestorac'] && $_REQUEST['unidadegestorac'][0] != '') {
            array_walk($_REQUEST['unidadegestorac'], 'quote');
            $sql_carregados = "SELECT ung.ungcod AS codigo,
                                      ung.ungcod || ' - ' || ung.ungdsc AS descricao
                                 FROM public.unidadegestora ung
                                 WHERE ung.ungcod IN(" . implode(',', $_REQUEST['unidadegestorac']) .")
                                 ORDER BY ung.ungabrev";
            $unidadegestorac = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT ung.ungcod AS codigo,
                         ung.ungcod || ' - ' || ung.ungdsc AS descricao
                    FROM public.unidadegestora ung
                    ORDER BY ung.ungabrev";
        mostrarComboPopup( 'Unidade Gestora do Cedente:', 'unidadegestorac', $stSql, '', 'Selecione a(s) Unidades(s) Gestora(s)');

        // -- Estado da documentação
        $esdid = array();
        if ($_REQUEST['esdid'] && $_REQUEST['esdid'][0] != '') {
            $sql_carregados = "SELECT esd.esdid AS codigo,
                                      esd.esdid || ' - ' ||esd.esddsc AS descricao
                                 FROM workflow.estadodocumento esd
                                 WHERE esd.tpdid = 97
                                   AND esd.esdid IN(" . implode(',', $_REQUEST['esdid']) . ")
                                 ORDER BY esddsc";
            $esdid = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT esd.esdid AS codigo,
                         esd.esdid || ' - ' ||esd.esddsc AS descricao
                    FROM workflow.estadodocumento esd
                    WHERE esd.tpdid = 97
                    ORDER BY esddsc";
        mostrarComboPopup( 'Estado da Documentação:', 'esdid', $stSql, '', 'Selecione o(s) Estados(s) da Documentação(ões)');

        // -- Coordenação
        $cooid = array();
        if ($_REQUEST['cooid'] && $_REQUEST['cooid'][0] != '') {
            $sql_carregados = "SELECT coo.cooid AS codigo,
                                      coo.cooid || ' - '|| coo.coodsc AS descricao
                                 FROM elabrev.coordenacao coo
                                 WHERE coo.coostatus = 'A'
                                   AND coo.cooid IN(" . implode(',', $_REQUEST['cooid']) . ")
                                 ORDER BY coo.cooid, coo.coodsc";
            $cooid = $db->carregar($sql_carregados);
        }
        $stSql = "SELECT coo.cooid AS codigo,
                         coo.cooid || ' - '|| coo.coodsc AS descricao
                    FROM elabrev.coordenacao coo
                    WHERE coo.coostatus = 'A'
                    ORDER BY 1, 2";
        mostrarComboPopup('Coordenação:', 'cooid', $stSql, '', 'Coordenação(ões)');
        ?>
        <td class="SubTituloDireita">Tipo Relatório:</td>
        <td>
            <input type="radio" name="tiporel" value="1" id="tiporel_1" <?php if($tiporel=='1') echo "checked"; ?>> 
            <label for="tiporel_1" style="cursor: pointer">Detalhado</label>
            &nbsp;&nbsp;&nbsp;
            <input type="radio" name="tiporel" value="2"id="tiporel_2" <?php if($tiporel=='2' || !$tiporel) echo "checked"; ?>>
            <label for="tiporel_2" style="cursor: pointer">Resumido</label>
        </td>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
                  &nbsp;&nbsp;&nbsp;
                 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeralXLS();" style="cursor: pointer;"/>
                  &nbsp;&nbsp;&nbsp;
                 <input type="button" value="Salvar Consulta" onclick="exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
            </td>
        </tr>
	</table>
</form>

<? if($carregarAux) {?>
    <script>
        onOffBloco('outros' );
        onOffBloco('minhasconsultas' );
    </script>
<?}?>