<?php
/**
 * Relatório geral do Módulo Programação Orçamentária.
 */

/**
 * Funções de apoio à criação do relatório.
 * @see geral.php
 */
require_once(APPRAIZ . 'www/minc/relatorio/geral.php');

header('content-type: text/html; charset=ISO-8859-1');

$hrefRedir = '/minc.php?modulo=relatorio/gerenciais/geral&acao=A';
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
    die();
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
    require_once("relatorio/geral_output.inc");
    exit;
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
    var ordenarcampo = document.getElementById('ordenarcampo');

    // Tipo de relatorio
    formulario.pesquisa.value = '1';
    prepara_formulario();
    selectAllOptions(formulario.agrupador);
    selectAllOptions(ordenarcampo);

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
            selectAllOptions(agrupador);
            selectAllOptions(ordenarcampo);

            formulario.target = 'resultadoGeral';
            var janela = window.open('', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        } else if (tipo == 'salvar') {
            if (formulario.titulo.value == '') {
                alert('É necessário informar o título do relatório!');
                formulario.titulo.focus();
                return;
            }
            var nomesExistentes = new Array();
            <?php
            $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
            $nomesExistentes = $db->carregar($sqlNomesConsulta);
            if ($nomesExistentes){
                foreach ($nomesExistentes as $linhaNome) {
                    print "nomesExistentes[nomesExistentes.length] = '" . str_replace("'", "\'", $linhaNome['prtdsc']) . "';";
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
            formulario.pesquisa.value = 0;
            formulario.target = '_self';
            formulario.action = 'minc.php?modulo=relatorio/gerenciais/geral&acao=A&salvar=1';
        }
    }
    formulario.submit();
}

function exibeRelatorioGeralXLS()
{
    var formulario = document.formulario;
    var agrupador = document.getElementById('agrupador');
    var ordenarcampo = document.getElementById('ordenarcampo');

    // Tipo de relatorio
    formulario.pesquisa.value = '2';

    prepara_formulario();
    selectAllOptions(formulario.agrupador);

    if (!agrupador.options.length) {
        alert('Favor selecionar ao menos um item para agrupar o resultado!');
        return false;
    }

    selectAllOptions(agrupador);
    selectAllOptions(ordenarcampo);
    formulario.submit();
}
	function tornar_publico(prtid){
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
        } catch(e) {}
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
	function onOffCampo(campo) {
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if (div_on.style.display == 'none') {
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		} else {
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
            $agrupador = new Agrupador('filtro', '');

            // Dados padrão de destino (nulo)
            $destino = array();
            $destino = $agrupadorAux;
            $origem = array(
                array('codigo' => 'estuf', 'descricao' => $arLabels['estuf']),
                array('codigo' => 'situacao', 'descricao' => $arLabels['situacao']),
                array('codigo' => 'mcemodalidadeensino', 'descricao' => $arLabels['mcemodalidadeensino']),
                array('codigo' => 'classificacao', 'descricao' => $arLabels['classificacao']),
                array('codigo' => 'eixo', 'descricao' => $arLabels['eixo']),
                array('codigo' => 'tipo', 'descricao' => $arLabels['tipo']),
            );

            // exibe agrupador
            $agrupador->setOrigem('naoAgrupador', null, $origem);
            $agrupador->setDestino('agrupador', null, $destino);
            $agrupador->exibir();
            ?>
            </td>
        </tr>
    </table>			
    <!-- OUTROS FILTROS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none">
        <tr>
            <td onclick="javascript:onOffBloco('outros');">
                <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                Relatórios Gerenciais
                <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="outros_div_filtros_off"></div>
	
    <div id="outros_div_filtros_on" style="display:none">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none">
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
    <!-- FIM OUTROS FILTROS -->
    <!-- MINHAS CONSULTAS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none">
        <tr>
            <td onclick="javascript:onOffBloco( 'minhasconsultas' );">
                <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                Minhas Consultas
                <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="minhasconsultas_div_filtros_off"></div>
    <div id="minhasconsultas_div_filtros_on" style="display:none">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none">
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
        <?php
        // -- Situação
        $situacao = array();
        if ($_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '') {
            $esdids = implode(',', $_REQUEST['situacao']);
            $sql_carregados = <<<DML
SELECT esdid AS codigo,
       esddsc AS descricao
  FROM workflow.estadodocumento et
    INNER JOIN workflow.tipodocumento tp
      ON (tp.tpdid = et.tpdid AND sisid = 143)
  WHERE et.tpdid = 80
    AND esdid IN({$esdids})
  ORDER BY esdordem
DML;
            $situacao = $db->carregar($sql_carregados);
        }
        $sql = <<<DML
SELECT esdid AS codigo,
       esddsc AS descricao
  FROM workflow.estadodocumento et
    INNER JOIN workflow.tipodocumento tp
      ON (tp.tpdid = et.tpdid AND sisid = 143)
  WHERE et.tpdid = 80
  ORDER BY esdordem
DML;
        mostrarComboPopup('Situação:', 'situacao', $sql, '', 'Selecione a(s) Situação(ões)');

        // -- Tipo
        $tipo = array();
        if ($_REQUEST['tipo'] && $_REQUEST['tipo'][0] != '') {
            $tipos = implode(',', $_REQUEST['tipo']);
            $tipos = explode(',', $tipos);
            array_walk($tipos, 'quote');
            $tipos = implode(',', $tipos);

            $sql = <<<DML
SELECT *
  FROM (SELECT '1,3' AS codigo,
	           'Todas' AS descricao
        UNION ALL
        SELECT '1' AS codigo,
	           'Estadual' AS descricao
        UNION ALL
        SELECT '3' AS codigo,
	           'Municipal' AS descricao) tipos
  WHERE codigo IN({$tipos})
DML;
            $tipo = $db->carregar($sql);
        }
        $sql = <<<DML
SELECT '1,3' AS codigo,
       'Todas' AS descricao
UNION ALL
SELECT '1' AS codigo,
       'Estadual' AS descricao
UNION ALL
SELECT '3' AS codigo,
       'Municipal' AS descricao
DML;
        mostrarComboPopup('Tipo:', 'tipo', $sql, '', 'Selecione o(s) Tipo(s)');

        // -- Modalidade de Ensino
        $mcemodalidadeensino = array();
        if ($_REQUEST['mcemodalidadeensino'] && $_REQUEST['mcemodalidadeensino'][0] != '') {
            array_walk($_REQUEST['mcemodalidadeensino'], 'quote');
            $selecionados = implode(',', $_REQUEST['mcemodalidadeensino']);
            $sql = <<<DML
SELECT *
  FROM (SELECT 'F' AS codigo,
	           'Ensino Fundamental' AS descricao
        UNION ALL
        SELECT 'M' AS codigo,
	           'Ensino Médio' AS descricao) tipos
  WHERE codigo IN({$selecionados})
DML;
            $mcemodalidadeensino = $db->carregar($sql);
        }
        $sql = <<<DML
SELECT 'F' AS codigo,
       'Ensino Fundamental' AS descricao
UNION ALL
SELECT 'M' AS codigo,
       'Ensino Médio' AS descricao
DML;
        mostrarComboPopup($arLabels['mcemodalidadeensino'] . ':', 'mcemodalidadeensino', $sql, '', 'Selecione a(s) Modalidade(s) de Ensino');

        // -- Classificação
        $classificacao = array();
        if ($_REQUEST['classificacao'] && $_REQUEST['classificacao'][0] != '') {
            array_walk($_REQUEST['classificacao'], 'quote');
            $selecionados = implode(',', $_REQUEST['classificacao']);
            $sql = <<<DML
SELECT *
  FROM (SELECT 'U' AS codigo,
	           'Urbana' AS descricao
        UNION ALL
        SELECT 'R' AS codigo,
	           'Rural' AS descricao
        UNION ALL
        SELECT 'A' AS codigo,
	           'Aberta' AS descricao) classificacoes
  WHERE codigo IN({$selecionados})
DML;
            $classificacao = $db->carregar($sql);
        }
        $sql = <<<DML
SELECT 'U' AS codigo,
	   'Urbana' AS descricao
UNION ALL
SELECT 'R' AS codigo,
	   'Rural' AS descricao
UNION ALL
SELECT 'A' AS codigo,
	   'Aberta' AS descricao
DML;
        mostrarComboPopup($arLabels['classificacao'].':', 'classificacao', $sql, '', 'Selecione a(s) Classificação(ções)');

        // -- Eixo
        $eixo = array();
        if ($_REQUEST['eixo'] && $_REQUEST['eixo'][0] != '') {
            $selecionados = implode(',', $_REQUEST['eixo']);
            $sql = <<<DML
SELECT extid AS codigo,
	   extdescricao AS descricao
  FROM minc.mceeixotematico
  WHERE extid IN({$selecionados})
  ORDER BY extdescricao
DML;
            $eixo = $db->carregar($sql);
        }
        $sql = <<<DML
SELECT extid AS codigo,
	   extdescricao AS descricao
  FROM minc.mceeixotematico
  ORDER BY extdescricao
DML;
        mostrarComboPopup("{$arLabels['eixo']}:", 'eixo', $sql, '', 'Selecione o(s) Eixo(s)');
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