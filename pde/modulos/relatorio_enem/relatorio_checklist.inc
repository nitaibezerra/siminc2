<?php
    /* Componente Historico de consulta.
     * Salva os POST da Consulta efetuado
     * Código implementado em 12/04/2013.
     * Luciano F. Ribeiro.
     */
    if ($_REQUEST['requisicao'] == 'salvarConsulta') {
        $existe_rel = 0;

        $sql = "select prtid from public.parametros_tela where prtdsc = '" . $_POST['titulo_consulta'] . "' and usucpf ='" . $_SESSION['usucpf'] . "'and mnuid =" . $_SESSION['mnuid'] . ";";

        $existe_rel = $db->pegaUm($sql);

        if ($existe_rel > 0) {
            $sql = sprintf(
                    "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d, prtobservacao = '%s' WHERE prtid = %d", $_POST['titulo_consulta'], addslashes(addslashes(serialize($_REQUEST))), $_SESSION['usucpf'], $_SESSION['mnuid'], $_POST['obs_consulta'], $existe_rel
            );
            $db->executar($sql);
            $db->commit();
        } else {
            $sql = sprintf(
                    "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid, prtobservacao) VALUES ( '%s', '%s', %s, '%s', %d, '%s' )", $_POST['titulo_consulta'], addslashes(addslashes(serialize($_REQUEST))), 'FALSE', $_SESSION['usucpf'], $_SESSION['mnuid'], $_POST['obs_consulta']
            );
            $db->executar($sql);
            $db->commit();
        }
        $db->sucesso('relatorio_enem/relatorio_checklist');
        die;
    }

    /* Componente Historico de consulta.
     * Transforma consulta em pública.
     * Código implementado em 12/04/2013.
     * Luciano F. Ribeiro.
     */
    if ($_REQUEST['prtid'] && $_REQUEST['publico']) {
        $sql = sprintf(
                "UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d", $_REQUEST['prtid']
        );
        $db->executar($sql);
        $db->commit();
        $db->sucesso('relatorio_enem/relatorio_checklist');
        die;
    }

    /* Componente Historico de consulta.
     * Exclui consulta.
     * Código implementado em 12/04/2013.
     * Luciano F. Ribeiro.
     */
    if ($_REQUEST['prtid'] && $_REQUEST['excluir'] == 1) {
        $sql = sprintf(
                "DELETE from public.parametros_tela WHERE prtid = %d", $_REQUEST['prtid']
        );
        $db->executar($sql);
        $db->commit();
        $db->sucesso('relatorio_enem/relatorio_checklist');
        die;
    }

    /* Componente Historico de consulta.
     * Carrega a consulta salva no historico, consulta dados em banco.
     * Código implementado em 12/04/2013.
     * Luciano F. Ribeiro.
     */
    if ($_REQUEST['prtid'] && $_REQUEST['carregar'] == 1) {

        $sql = sprintf("select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid']);

        $itens = $db->pegaUm($sql);
        $dados = unserialize(stripslashes(stripslashes($itens)));

        extract($dados);
        $_REQUEST = $dados;
        $_POST = $dados;
    }

    #exibe consulta
    if (isset($_REQUEST['form']) == true) {
        include "checklist_resultado.inc";
        exit;
    }

    include APPRAIZ . 'includes/cabecalho.inc';
    include APPRAIZ . 'includes/Agrupador.php';

    echo "<br>";
    $titulo_modulo = "Relatório Checklist";
    monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');
    
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

    //BLOCO DE CÓDIGO - HISTORICO DE CONSULTAS.	
    function tornar_publico( prtid ){
        document.filtro.publico.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

    function excluir_relatorio( prtid ){
        document.filtro.excluir.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

    function carregar_consulta( prtid ){
        document.filtro.carregar.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

    function carregar_relatorio( prtid ){
        document.getElementById('carregar').value = '1';
        document.getElementById('prtid').value = prtid;
        document.getElementById('filtro').action = 'enem.php?modulo=relatorio_enem/relatorio_checklist&acao=A';
        document.getElementById('filtro').target = '_blank';
        document.getElementById('filtro').submit();
    }
	
    function salvarConsultaEfetuada(){
        var formulario = document.filtro;
        var agrupador = document.getElementById('agrupadores');

        if ( document.getElementById('titulo_consulta').value == '' ) {
            alert( 'É necessário informar a descrição do relatório!' );
            document.getElementById('titulo_consulta').focus();
            return;
        }

        jQuery("#div_selecao").css("visibility", "visible");

        var nomesExistentes = new Array();
        <?php
            $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela Where usucpf ='" . $_SESSION['usucpf'] . "'and mnuid =" . $_SESSION['mnuid'] . ";";
            $nomesExistentes = $db->carregar($sqlNomesConsulta);
            if ($nomesExistentes) {
                foreach ($nomesExistentes as $linhaNome) {
                    print "nomesExistentes[nomesExistentes.length] = '" . str_replace("'", "\'", $linhaNome['prtdsc']) . "';";
                }
            }
        ?>
        var confirma = true;
        var i, j = nomesExistentes.length;
        for(i = 0; i < j; i++){
            if(nomesExistentes[i] == document.getElementById('titulo_consulta').value){
                confirma = confirm( 'Deseja alterar a consulta já existente?');
                break;
            }
        }
        if(!confirma){
            return;
        }

        selectAllOptions(agrupador);
        selectAllOptions(document.getElementById('atividade'));
        selectAllOptions(document.getElementById('etapa'));
        selectAllOptions(document.getElementById('processo'));
        selectAllOptions(document.getElementById('subprocesso'));
        selectAllOptions(document.getElementById('executores'));
        selectAllOptions(document.getElementById('validadores'));
        selectAllOptions(document.getElementById('certificadores'));
        selectAllOptions(document.getElementById('destinatarios'));
    }
    
    function escolhaRelatorio(visual){
        if(visual == 'html'){
            document.getElementById('tipo_relatorio').value = 'visual';
        }else{
            document.getElementById('tipo_relatorio').value = 'xls';
        }

        jQuery("#div_selecao").css("visibility", "hidden");
        document.getElementById('filtro').action = 'enem.php?modulo=relatorio_enem/relatorio_checklist&acao=A&requisicao=salvarConsulta';
        document.getElementById('filtro').target = '_self';
        document.getElementById('filtro').submit();
    }
    //FIM DO BLOCO DE CÓDIGO - HISTORICO DE CONSULTAS.


    function checklist_exibeRelatorioGeral(tipo) {

        var formulario = document.filtro;
        var agrupador = document.getElementById('agrupadores');

        // Tipo de relatorio
        formulario.pesquisa.value = '1';


        prepara_formulario();
        selectAllOptions(formulario.agrupadores);

        if (tipo == 'exibir') {

            if (!agrupador.options.length) {
                alert('Favor selecionar ao menos um agrupador!');
                return false;
            }

            selectAllOptions(agrupador);
            selectAllOptions(document.getElementById('atividade'));
            selectAllOptions(document.getElementById('etapa'));
            selectAllOptions(document.getElementById('processo'));
            selectAllOptions(document.getElementById('subprocesso'));
            selectAllOptions(document.getElementById('executores'));
            selectAllOptions(document.getElementById('validadores'));
            selectAllOptions(document.getElementById('certificadores'));
            selectAllOptions(document.getElementById('destinatarios'));

            formulario.target = 'resultadoChecklistGeral';
            var janela = window.open('', 'resultadoChecklistGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();

        }

        if (tipo == 'exibirxls') {

            formulario.pesquisa.value = '2';

            if (!agrupador.options.length) {
                alert('Favor selecionar ao menos um agrupador!');
                return false;
            }

            selectAllOptions(agrupador);
            selectAllOptions(document.getElementById('atividade'));
            selectAllOptions(document.getElementById('etapa'));
            selectAllOptions(document.getElementById('processo'));
            selectAllOptions(document.getElementById('subprocesso'));
            selectAllOptions(document.getElementById('executores'));
            selectAllOptions(document.getElementById('validadores'));
            selectAllOptions(document.getElementById('certificadores'));
            selectAllOptions(document.getElementById('destinatarios'));

        }

        formulario.submit();

    }


    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }


    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
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
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
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

</script>

<style type="text/css">
    #div_selecao {
        visibility: hidden;
        border-style: solid;
        border-width: 1px;
        borborder-color: black;
        background-color: #DCDCDC;
        width: 23%;
        height: 14%;
        position: absolute;
        top: 58%;
        left: 35%; 
    }
</style>  

<form action="" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>

	<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<input type="hidden" id="exibir" name="exibir" value="">
    
    <!-- HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none; border-top:none;">
        <tr>
            <td colspan="2" class="SubTituloEsquerda">HISTORICO DE CONSULTAS:</td>
        </tr>
        <tr>
            <td style="width:10%;">Definir título da consulta:</td>
            <td>
                <?= campo_texto( 'titulo_consulta', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo_consulta"'); ?>
            </td>
        </tr>
    </table>
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco('minhasconsultas');" >  
                <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                Minhas Consultas
                <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />					
            </td>
        </tr>
    </table>

    <div name="minhasconsultas_div_filtros_off" id="minhasconsultas_div_filtros_off"></div>
    <div id="minhasconsultas_div_filtros_on" style="display:none;">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
            <tr>
                <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                <?php
                    $sql = sprintf("
                            Select Case When prtpublico = false 
                                        Then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
                                        Else '<img border=\"0\" src=\"../imagens/grupo_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
                                   End as acao, 
                                   '<b>' || prtdsc || '</b>' as descricao,
                                   '<b>' || prtobservacao || '</b>' as observacao
                            From public.parametros_tela 
                            Where mnuid = %d AND usucpf = '%s'", $_SESSION['mnuid'], $_SESSION['usucpf']
                    );
                    $cabecalho = array('Ação', 'Nome', 'Observações');
                ?>
                <td>
                <?php $db->monta_lista_simples($sql, $cabecalho, 50, 50, 'N', '95%', null); ?>
                </td>
            </tr>
        </table>
    </div>
    <!-- FIM - HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->    

    <!-- CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco('outros');">
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
                <td>
                    <?php
                    $sql = sprintf("
                            Select  Case When prtpublico = true and usucpf = '%s' 
                                        Then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
                                              <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
                                              <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
                                        Else '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
                                             <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'  
                                    End as acao, 
                                    '<b>' || prtdsc || '</b>' as descricao,
                                    '<b>' || prtobservacao || '</b>' as observacao
                            From public.parametros_tela 
                            Where mnuid = %d and prtpublico = TRUE
                    ", $_SESSION['usucpf'], $_SESSION['mnuid'], $_SESSION['usucpf']);

                    $cabecalho = array('Ação', 'Nome', 'Observações');
                    $db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null);
                    ?>
                </td>
            </tr>
        </table>
    </div>
    <!-- FIM DA CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->
    
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td colspan="2" class="SubTituloEsquerda">AGRUPADORES:</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Agrupadores</td>
            <td>
<?php
// Início dos agrupadores
$agrupador = new Agrupador('filtro', '');

// Dados padrão de destino (nulo)
$destino = isset($agrupador2) ? $agrupador2 : array();

// Dados padrão de origem
$origem = array(
    'itemprazo' => array(
        'codigo' => 'itemprazo',
        'descricao' => 'Prazo do item'
    ),
    'itemcritico' => array(
        'codigo' => 'itemcritico',
        'descricao' => 'Crítico'
    ),
    'execucao_agrupador' => array(
        'codigo' => 'execucao_agrupador',
        'descricao' => 'Execução'
    ),
    'executores' => array(
        'codigo' => 'executores',
        'descricao' => 'Executor(es)'
    ),
    'validacao_agrupador' => array(
        'codigo' => 'validacao_agrupador',
        'descricao' => 'Validação'
    ),
    'validadores' => array(
        'codigo' => 'validadores',
        'descricao' => 'Validador(es)'
    ),
    'certificacao_agrupador' => array(
        'codigo' => 'certificacao_agrupador',
        'descricao' => 'Certificação'
    ),
    'certificadores' => array(
        'codigo' => 'certificadores',
        'descricao' => 'Certificador(es)'
    ),
    'etapas' => array(
        'codigo' => 'etapas',
        'descricao' => 'Etapas'
    ),
    'processos' => array(
        'codigo' => 'processos',
        'descricao' => 'Processos'
    ),
    'subprocessos' => array(
        'codigo' => 'subprocessos',
        'descricao' => 'Sub-processos'
    ),
    'atividades' => array(
        'codigo' => 'atividades',
        'descricao' => 'Atividades'
    ),
    'pessoas' => array(
        'codigo' => 'pessoas',
        'descricao' => 'Pessoa'
    )
    ,
    'gestorprocesso' => array(
        'codigo' => 'gestorprocesso',
        'descricao' => 'Gestor de Processos'
    )
    ,
    'gestorrisco' => array(
        'codigo' => 'gestorrisco',
        'descricao' => 'Gestor de Risco'
    )
);

// exibe agrupador
$agrupador->setOrigem('naoAgrupadores', null, $origem);
$agrupador->setDestino('agrupadores', null, $destino);
$agrupador->exibir();
?>
            </td>
        </tr>
    </table>
    
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr><td colspan="2" class="SubTituloEsquerda">FILTROS:</td></tr>
<?php
// Etapas
$stSql = "  SELECT 
								CASE 
									WHEN ati3.atitipoenem = 'E' THEN ati3.atiid 
									WHEN ati4.atitipoenem = 'E' THEN ati4.atiid 
								END as codigo,
								CASE 
									WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
									WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao
								END as descricao 
							FROM 
								pde.itemchecklist icl
							LEFT JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							LEFT JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati4 ON ati4.atiid = ati4.atiidpai AND ati4.atistatus = 'A'
							ORDER BY 2";
$stSql = "SELECT 
									ati.atiid as codigo,
									ati._atinumero ||' - '|| ati.atidescricao as descricao 
							FROM pde.atividade ati --ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
								where ati.atistatus = 'A' and atitipoenem = 'E'
							ORDER BY ati._atinumero";
$stSqlCarregados = "";
mostrarComboPopup('Etapas', 'etapa', $stSql, $stSqlCarregados, 'Selecione a(s) Etapa(s)');

// Processos
$stSql = "  SELECT 
								CASE 
									WHEN ati2.atitipoenem = 'P' THEN ati2.atiid 
									WHEN ati3.atitipoenem = 'P' THEN ati3.atiid 
								END as codigo,
								CASE 
									WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
									WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao
								END as descricao 
							FROM 
								pde.itemchecklist icl
							LEFT JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							LEFT JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
							ORDER BY 2";
$stSqlCarregados = "";
mostrarComboPopup('Processos', 'processo', $stSql, $stSqlCarregados, 'Selecione o(s) Processo(s)');

// Sub-Processos
$stSql = "  SELECT 
								CASE 
									WHEN ati2.atitipoenem = 'S' THEN ati2.atiid 
								END as codigo,
								CASE 
									WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
								END as descricao
							FROM 
								pde.itemchecklist icl
							INNER JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							INNER JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							ORDER BY 2";
$stSqlCarregados = "";
mostrarComboPopup('Sub-processos', 'subprocesso', $stSql, $stSqlCarregados, 'Selecione o(s) Sub-processo(s)');

// Atividades
$stSql = " SELECT
								ati.atiid AS codigo,
								ati._atinumero||' - '||ati.atidescricao AS descricao
							FROM 
								pde.atividade ati 
							INNER JOIN 
								pde.itemchecklist icl ON icl.atiid = ati.atiid 
							GROUP BY
								ati.atiid, ati.atidescricao, ati._atinumero, ati._atiordem  
							ORDER BY
								ati._atiordem";
$stSqlCarregados = "";
mostrarComboPopup('Atividades', 'atividade', $stSql, $stSqlCarregados, 'Selecione a(s) Atividade(s)');

// Executor(es)
$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=1
							ORDER BY
								2";
mostrarComboPopup('Executor(es)', 'executores', $stSql, $stSqlCarregados, 'Selecione o(s) Executor(es)', Array(), '', '', '', Array('descricao'), Array('dsc'));

// Validador(es)
$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=2
							ORDER BY
								ent.entnome";
mostrarComboPopup('Validador(es)', 'validadores', $stSql, $stSqlCarregados, 'Selecione o(s) Validador(es)', Array(), '', '', '', Array('descricao'), Array('dsc'));

// Certificador(es)
$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=3
							ORDER BY
								ent.entnome";
mostrarComboPopup('Certificador(es)', 'certificadores', $stSql, $stSqlCarregados, 'Selecione o(s) Certificador(es)', Array(), '', '', '', Array('descricao'), Array('dsc'));

// Pessoa(s)
$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid in (1,2,3)
							ORDER BY
								ent.entnome";
mostrarComboPopup('Pessoa(s)', 'pessoas', $stSql, $stSqlCarregados, 'Selecione a(s) Pessoa(s)', Array(), '', '', '', Array('descricao'), Array('dsc'));

$stSql = "select
						  u.usucpf as codigo,
						  u.usucpf || ' - ' || u.usunome as descricao
						  from seguranca.usuario u
						  inner join pde.usuarioresponsabilidade ur on
						  ur.usucpf = u.usucpf
						  inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						  where
						  ur.rpustatus = 'A' and
						  ur.pflcod = '" . PERFIL_GERENTE . "'							
						  order by u.usunome";
mostrarComboPopup('Gestor de Processos', 'gestorProcessos', $stSql, $stSqlCarregados, 'Selecione o(s) Gestor(es) de Processo', Array(), '', '', '', Array('descricao'), Array('dsc'));

$stSql = "select
						  u.usucpf as codigo,
						  u.usucpf || ' - ' || u.usunome as descricao
						  from seguranca.usuario u
						  inner join pde.usuarioresponsabilidade ur on
						  ur.usucpf = u.usucpf
						  inner join seguranca.perfilusuario pu on pu.pflcod = ur.pflcod and pu.usucpf = ur.usucpf
						  where
						  ur.rpustatus = 'A' and
						  ur.pflcod = '" . PERFIL_EQUIPE_APOIO_GERENTE . "'
						  order by u.usunome";
mostrarComboPopup('Gestor de Risco', 'gestorRiscos', $stSql, $stSqlCarregados, 'Selecione o(s) Gestor(es) de Risco', Array(), '', '', '', Array('descricao'), Array('dsc'));
?>
        <tr>
            <td class="SubTituloEsquerda" colspan="2">PENDÊNCIAS:</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Executado:</td>
            <td><input type="radio" name="possuiexecucao" value="" checked> Não aplica <input type="radio" name="possuiexecucao" value="nao" > Não <input type="radio" name="possuiexecucao" value="sim"> Sim</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Validado:</td>
            <td><input type="radio" name="possuivalidacao" value="" checked> Não aplica <input type="radio" name="possuivalidacao" value="nao" > Não <input type="radio" name="possuivalidacao" value="sim"> Sim</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Certificado:</td>
            <td><input type="radio" name="possuicertificacao" value="" checked> Não aplica <input type="radio" name="possuicertificacao" value="nao" > Não <input type="radio" name="possuicertificacao" value="sim"> Sim</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo vencido:</td>
            <td><input type="radio" name="prazovencido" value="nao" checked> Não <input type="radio" name="prazovencido" value="sim"> Sim</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo de Execução:</td>
            <td>  
<?= campo_data2('data_inicio_exec', 'N', 'S', 'Data Início', '##/##/####') ?>&nbsp;até&nbsp;
<?= campo_data2('data_fim_exec', 'N', 'S', 'Data Fim', '##/##/####') ?>
            </td> 
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo de Validação:</td>
            <td>  
<?= campo_data2('data_inicio_val', 'N', 'S', 'Data Início', '##/##/####') ?>&nbsp;até&nbsp;
<?= campo_data2('data_fim_val', 'N', 'S', 'Data Fim', '##/##/####') ?>
            </td> 
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo de Certificação:</td>
            <td>  
<?= campo_data2('data_inicio_cert', 'N', 'S', 'Data Início', '##/##/####') ?>&nbsp;até&nbsp;
<?= campo_data2('data_fim_cert', 'N', 'S', 'Data Fim', '##/##/####') ?>
            </td> 
        </tr>
        <tr>
            <td class="SubTituloDireita">Prazo final:</td>
            <td>  
<?= campo_data2('data_inicio', 'N', 'S', 'Data Início', '##/##/####') ?>&nbsp;até&nbsp;
<?= campo_data2('data_fim', 'N', 'S', 'Data Fim', '##/##/####') ?>
            </td> 
        </tr>
        <tr>
            <td class="SubTituloDireita">Relatório Aberto/Fechado:</td>
            <td>
                <input type="radio" name="arvore_" value="true" checked> Aberto 
                <input type="radio" name="arvore_" value=""> Fechado
            </td>
        </tr>
        <tr>
            <td class="SubTituloEsquerda" colspan="2">ENVIDO DE EMAIL:</td>
        </tr>
<?php
// Destinatário(s)
$stSql = " SELECT
							ent.entid AS codigo,
							ent.entnome AS descricao
						FROM 
							entidade.entidade ent 
						INNER JOIN 
							pde.checklistentidade cle ON cle.entid = ent.entid
						WHERE
							cle.tpvid in (1,2,3)
						ORDER BY
							ent.entnome";
mostrarComboPopup('Destinatário(s)', 'destinatarios', $stSql, $stSqlCarregados, 'Selecione o(s) Destinatário(s)', Array(), '', '', '', Array('descricao'), Array('dsc'));
?>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar" onclick="checklist_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
                <input type="button" value="Visualizar XLS" onclick="checklist_exibeRelatorioGeral('exibirxls');" style="cursor: pointer;"/>
                <input type="button" value="Salvar Consulta" onclick="salvarConsultaEfetuada();" style="cursor: pointer;"/>
            </td>
        </tr>
    </table>
    <div id="div_selecao" name="div_selecao">
            <br><b style="margin-left: 6%;">Selecione o tipo de relatório que deseja que seja gerado</b><br>
            <input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_html" onclick="document.getElementById('exibir').value='relatorio_html';escolhaRelatorio('html');">Relatório em HTML <br/>
            <input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_xls" onclick="document.getElementById('exibir').value='relatorio_xls';escolhaRelatorio('xls');">Relatório em XLS
    </div>
</form>