<?php
//ver($_REQUEST,d);
if ($_REQUEST['reqAjax']) {
    $_REQUEST['reqAjax']($_REQUEST);
    die();
}

if (isset($_POST['tipo_ensino'])) {

    // Programa
    $stSql = "SELECT
							prfid AS codigo,
							prfdesc AS descricao
						 FROM 
							obras.programafonte
							" . (!empty($_POST['orgid']) ? 'WHERE orgid IN (' . $_POST['orgid'] . ')' : '') . "
						 ORDER BY
							prfdesc ";
    mostrarComboPopup('Programa', 'prfid', $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)');
    /*
                // Fonte
                $stSql = "SELECT
                                tooid AS codigo,
                                toodescricao AS descricao
                            FROM
                                  obras.tipoorigemobra
                            WHERE
                                  toostatus = 'A'
                                  ".(!empty($_POST['orgid']) ? 'AND orgid IN ('.$_POST['orgid'].')' : '')."
                          ORDER BY
                                  toodescricao ";

                mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $stSqlCarregados, 'Selecione a(s) Fonte(s)' );
    */
    die;
}

// transforma consulta em pública
if ($_REQUEST['prtid'] && $_REQUEST['publico']) {
    $sql = sprintf(
        "UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
        $_REQUEST['prtid']
    );
    $db->executar($sql);
    $db->commit();
    ?>
    <script type="text/javascript">
        location.href = '?modulo=<?= $modulo ?>&acao=A';
    </script>
    <?
    die;
}
// FIM transforma consulta em pública

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
        location.href = '?modulo=<?= $modulo ?>&acao=A';
    </script>
    <?
    die;
}
// FIM remove consulta

// remove flag de submissão de formulário
if ($_REQUEST['prtid'] && $_REQUEST['carregar']) {
    unset($_REQUEST['form']);
}
// FIM remove flag de submissão de formulário

// exibe consulta
if (isset($_REQUEST['form']) == true) {
     if ($_REQUEST['prtid']) {
        $sql = sprintf("select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid']);
        $itens = $db->pegaUm($sql);
        $dados = unserialize(stripslashes(stripslashes($itens)));
        $_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
        unset($_REQUEST['salvar']);
    }

    switch ($_REQUEST['pesquisa']) {

        case '1':
            include "geral_resultado.inc";
            exit;
        case '2':
            include "geralxls_resultado.inc";
            exit;
    }

}

// carrega consulta do banco
if ($_REQUEST['prtid'] && $_REQUEST['carregar'] == 1) {

    $sql = sprintf("select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid']);
    $itens = $db->pegaUm($sql);
    $dados = unserialize(stripslashes(stripslashes($itens)));
    extract($dados);
    $_REQUEST = $dados;
    unset($_REQUEST['form']);
    unset($_REQUEST['pesquisa']);
    $titulo = $_REQUEST['titulo'];

    $agrupador2 = array();

    if ($_REQUEST['agrupador']) {

        foreach ($_REQUEST['agrupador'] as $valorAgrupador) {
            array_push($agrupador2, array('codigo' => $valorAgrupador, 'descricao' => $valorAgrupador));
        }

    }

}


if (isset($_REQUEST['pesquisa']) || isset($_REQUEST['tipoRelatorio'])) {
    switch ($_REQUEST['pesquisa']) {
        case '1':
            include "geral_resultado.inc";
            exit;
        case '2':
            include "geralxls_resultado.inc";
            exit;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela, $url, '');
$titulo_modulo = "Relatório Quantitativo";
monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');

if ($_REQUEST['filtroagrupador']) {

    switch ($_REQUEST['filtroagrupador']) {
        case '1':
            $agrupador2 = array(
                'nivelpreenchimento' => array(
                    'codigo' => 'nivelpreenchimento',
                    'descricao' => 'Nível de Preenchimento'
                ),
                'situacao' => array(
                    'codigo' => 'situacao',
                    'descricao' => 'Situação da Obra'
                ),
                'regiao' => array(
                    'codigo' => 'regiao',
                    'descricao' => 'Região'
                ),
                'uf' => array(
                    'codigo' => 'uf',
                    'descricao' => 'UF'
                ),
                'municipio' => array(
                    'codigo' => 'municipio',
                    'descricao' => 'Município'
                ),
                'nomedaobra' => array(
                    'codigo' => 'nomedaobra',
                    'descricao' => 'Nome da Obra'
                )
            );
            break;
        case '2':
            $agrupador2 = array(
                'regiao' => array(
                    'codigo' => 'regiao',
                    'descricao' => 'Região'
                ),
                'uf' => array(
                    'codigo' => 'uf',
                    'descricao' => 'UF'
                ),
                'municipio' => array(
                    'codigo' => 'municipio',
                    'descricao' => 'Município'
                ),
                'nomedaobra' => array(
                    'codigo' => 'nomedaobra',
                    'descricao' => 'Nome da Obra'
                )
            );
            break;
        case '3':
            $agrupador2 = array(
                'situacao' => array(
                    'codigo' => 'situacao',
                    'descricao' => 'Situação da Obra'
                ),
                'tipodaobra' => array(
                    'codigo' => 'tipodaobra',
                    'descricao' => 'Tipo da Obra'
                ),
                'regiao' => array(
                    'codigo' => 'regiao',
                    'descricao' => 'Região'
                ),
                'uf' => array(
                    'codigo' => 'uf',
                    'descricao' => 'UF'
                ),
                'municipio' => array(
                    'codigo' => 'municipio',
                    'descricao' => 'Município'
                ),
                'nomedaobra' => array(
                    'codigo' => 'nomedaobra',
                    'descricao' => 'Nome da Obra'
                )
            );
            break;
        case '4':
            $agrupador2 = array(
                'situacao' => array(
                    'codigo' => 'situacao',
                    'descricao' => 'Situação da Obra'
                ),
                'regiao' => array(
                    'codigo' => 'regiao',
                    'descricao' => 'Região'
                ),
                'uf' => array(
                    'codigo' => 'uf',
                    'descricao' => 'UF'
                ),
                'municipio' => array(
                    'codigo' => 'municipio',
                    'descricao' => 'Município'
                ),
                'nomedaobra' => array(
                    'codigo' => 'nomedaobra',
                    'descricao' => 'Nome da Obra'
                )
            );
            break;
    }

    $orgid = $_REQUEST['orgid'];
    if ($_REQUEST['prfid']) {
        $stSqlCarregados_prf = "SELECT
									prfid AS codigo,
									prfdesc AS descricao
								FROM 
									obras.programafonte
								WHERE 
									prfid in (" . $_REQUEST['prfid'] . ")
								ORDER BY
									prfdesc";
    }

    if ($_REQUEST['tooid']) {
        $stSqlCarregados_too = "SELECT
									tooid AS codigo, 
									toodescricao AS descricao
  						  		FROM 
									obras.tipoorigemobra
  						  		WHERE
									tooid in (" . $_REQUEST['tooid'] . ")
						  		ORDER BY
									toodescricao";
    }

    if ($_REQUEST['stoid']) {
        $stSqlCarregados_sto = "SELECT
										stoid AS codigo,
										stodesc AS descricao
								FROM 
									obras.situacaoobra
								WHERE 
									stoid in (" . $_REQUEST['stoid'] . ")
								ORDER BY
									stodesc";
    }

    $script_est = "jQuery(document).ready(function() {" . (($situacao[1] == 'menor80') ? "jQuery('#percentualinicial').val('0');jQuery('#percentualfinal').val('80');" : "") . "" . (($situacao[1] == 'maior80') ? "jQuery('#percentualinicial').val('80');jQuery('#percentualfinal').val('100');" : "") . "jQuery('[name=orgid[]][value=$orgid]').attr('checked',true);obras_exibeRelatorioGeral('exibir');});";
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();

//divCarregando();

<?=$script_est ?>



//	jQuery(function(){
//				
//	});

function obras_exibeRelatorioGeralXLS() {

    var formulario = document.filtro;
    var agrupador = document.getElementById('agrupador');

    // Tipo de relatorio
    formulario.pesquisa.value = '2';

    //prepara_formulario();
    selectAllOptions(formulario.agrupador);


    if (!agrupador.options.length) {
        alert('Favor selecionar ao menos um item para agrupar o resultado!');
        return false;
    }

    selectAllOptions(agrupador);

    selectAllOptions(document.getElementById('unidade'));
    selectAllOptions(document.getElementById('campus'));
    selectAllOptions(document.getElementById('modulos'));
    selectAllOptions(document.getElementById('unidademed'));
    selectAllOptions(document.getElementById('ambiente'));

    formulario.submit();

}

function obras_exibeRelatorioGeral(tipo) {





    var formulario = document.filtro;
    var agrupador = document.getElementById('agrupador');

    // Tipo de relatorio
    formulario.pesquisa.value = '1';



    //prepara_formulario();
    selectAllOptions(formulario.agrupador);

    if (tipo == 'relatorio') {



        formulario.action = 'academico.php?modulo=relatorio/relatorio_geral_infra&acao=A';
       window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';

    } else {

        if (tipo == 'planilha') {


            if (!agrupador.options.length) {
                alert('Favor selecionar ao menos um item para agrupar o resultado!');
                return false;
            }

            formulario.action = 'academico.php?modulo=relatorio/relatorio_geral_infra&acao=A&tipoRelatorio=xls';

        } else if (tipo == 'salvar') {

            if (formulario.titulo.value == '') {
                alert('É necessário informar a descrição do relatório!');
                formulario.titulo.focus();
                return;
            }
            var nomesExistentes = new Array();
            <?php
                $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
                $nomesExistentes = $db->carregar( $sqlNomesConsulta );
                if ( $nomesExistentes ){
                    foreach ( $nomesExistentes as $linhaNome )
                    {
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
            formulario.action = 'academico.php?modulo=relatorio/relatorio_geral_infra&acao=A&salvar=1';
            formulario.target = '_self';

        } else if (tipo == 'exibir') {



//            if (!document.getElementsByName('orgid[]').item(0).checked && !document.getElementsByName('orgid[]').item(1).checked && !document.getElementsByName('orgid[]').item(2).checked) {
//                alert('Favor selecionar ao menos um tipo de ensino!');
//                return false;
//            }


            if (!agrupador.options.length) {
                alert('Favor selecionar ao menos um item para agrupar o resultado!');
                return false;
            }


            selectAllOptions(agrupador);

            selectAllOptions(document.getElementById('unidade'));
            selectAllOptions(document.getElementById('campus'));
            selectAllOptions(document.getElementById('modulos'));
            selectAllOptions(document.getElementById('unidademed'));
            selectAllOptions(document.getElementById('ambiente'));



            formulario.target = 'resultadoObrasGeral';
            var janela = window.open('', 'resultadoObrasGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            janela.focus();
        }
    }

    formulario.submit();

}


function tornar_publico(prtid) {
//		location.href = '?modulo=<?//= $modulo ?>&acao=R&prtid='+ prtid + '&publico=1';
    document.filtro.publico.value = '1';
    document.filtro.prtid.value = prtid;
    document.filtro.target = '_self';
    document.filtro.submit();
}

function excluir_relatorio(prtid) {
    document.filtro.excluir.value = '1';
    document.filtro.prtid.value = prtid;
    document.filtro.target = '_self';
    document.filtro.submit();
}

function carregar_consulta(prtid) {
    document.filtro.carregar.value = '1';
    document.filtro.prtid.value = prtid;
    document.filtro.target = '_self';
    document.filtro.submit();
}

function carregar_relatorio(prtid) {
    document.filtro.prtid.value = prtid;
    obras_exibeRelatorioGeral('relatorio');
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
/* Função para adicionar linha nas tabelas */

/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
function criarrequisicao() {
    return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Msxml2.XMLHTTP');
}
/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */

/* FUNÇÃO QUE TRATA O RETORNO */
var pegarretorno = function () {
    try {
        if (evXmlHttp.readyState == 4) {
            if (evXmlHttp.status == 200 && evXmlHttp.responseText != '') {
                // criando options
                var x = evXmlHttp.responseText.split("&&");
                for (i = 1; i < (x.length - 1); i++) {
                    var dados = x[i].split("##");
                    document.getElementById('usrs').options[i] = new Option(dados[1], dados[0]);
                }
                var dados = x[0].split("##");
                document.getElementById('usrs').options[0] = new Option(dados[1], dados[0]);
                document.getElementById('usrs').value = cpfselecionado;
            }
            if (evXmlHttp.dispose) {
                evXmlHttp.dispose();
            }
            evXmlHttp = null;
        }
    }
    catch (e) {
    }
};
/* FIM - FUNÇÃO QUE TRATA O RETORNO */


/**
 * Alterar visibilidade de um bloco.
 *
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco(bloco) {
    var div_on = document.getElementById(bloco + '_div_filtros_on');
    var div_off = document.getElementById(bloco + '_div_filtros_off');
    var img = document.getElementById(bloco + '_img');
    var input = document.getElementById(bloco + '_flag');
    if (div_on.style.display == 'none') {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '0';
        img.src = '/imagens/menos.gif';
    }
    else {
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
    var div_on = document.getElementById(campo + '_campo_on');
    var div_off = document.getElementById(campo + '_campo_off');
    var input = document.getElementById(campo + '_campo_flag');
    if (div_on.style.display == 'none') {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}
//-->
</script>
<form action="" method="post" name="filtro" id="filtro">
<input type="hidden" name="form" value="1"/>
<input type="hidden" name="pesquisa" value="1"/>
<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
<input type="hidden" name="prtid" value=""/>
<!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
        <td class="SubTituloDireita">Título</td>
        <td>
            <?= campo_texto('titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
        </td>
    </tr>
    <tr style="display: none">
        <td class="SubTituloDireita">Tipo de Ensino</td>
        <td style="display: none">
            <?php

            // Monta as opções de tipo de ensino de acordo com a responsabilidade do usuario
            if (($db->testa_superuser()) || (possuiPerfil(PERFIL_CONSULTAGERAL) ||
                    possuiPerfil(PERFIL_GESTORMEC) || possuiPerfil(PERFIL_ADMINISTRADOR))
            ) {

                $orgaos = $db->carregar("SELECT
													orgid, orgdesc 
												 FROM 
													academico.orgao");

                $count = count($orgaos);
                for ($i = 0; $i < $count; $i++) {
                  //  echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';
                }

            } else {

                $orgaos = obras_pegarOrgaoPermitido();
              //  echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[0]["id"] . '" checked=\"checked\"/>' . $orgaos[0]["descricao"] . '&nbsp;';

            }
            ?>
        </td>
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
                'pais' => array(
                    'codigo' => 'unidade',
                    'descricao' => 'Unidades'
                ),
                'uf' => array(
                    'codigo' => 'uf',
                    'descricao' => 'UF'
                ),
                'campus' => array(
                    'codigo' => 'campus',
                    'descricao' => 'Campus'
                ),
                'modulo' => array(
                    'codigo' => 'modulo',
                    'descricao' => 'Módulo'
                ),
                'ambiente' => array(
                    'codigo' => 'ambiente',
                    'descricao' => 'Ambiente'
                ),


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

<!--<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3"-->
<!--       style="border-bottom:none;border-top:none;">-->
<!--    <tr>-->
<!--        <td onclick="javascript:onOffBloco( 'outros' );">-->
<!--            <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;-->
<!--            Relatórios Gerenciais-->
<!--            <input type="hidden" id="outros_flag" name="outros_flag" value="0"/>-->
<!--        </td>-->
<!--    </tr>-->
<!--</table>-->
<!--<div id="outros_div_filtros_off">-->
    <!--
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
        </tr>
    </table>
    -->
<!--</div>-->
<!---->
<!--<div id="outros_div_filtros_on" style="display:none;">-->
<!--    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">-->
<!--        <tr>-->
<!--            <td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>-->
<!--            --><?php
//
//            if ($db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) ||
//                possuiPerfil(PERFIL_ADMINISTRADOR)
//            ) {
//                $bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
//            }
//
//            $sql = sprintf(
//                "SELECT Case when prtpublico = true and usucpf = '%s' then '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
//                $_SESSION['usucpf'],
//                $_SESSION['mnuid'],
//                $_SESSION['usucpf']
//            );
//            /*
//            $sql = sprintf(
//                "SELECT 'abc' as acao, prtdsc FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
//                $_SESSION['mnuid']
//            );
//            */
//            $cabecalho = array('Ação', 'Nome');
//            ?>
<!--            <td>--><?php //$db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null); ?>
<!--            </td>-->
<!--        </tr>-->
<!--    </table>-->
<!--</div>-->
<!--<script language="javascript">    //alert( document.formulario.agrupador_combo.value );	</script>-->
<!---->
<!--<!-- FIM OUTROS FILTROS -->
<!---->
<!--<!-- MINHAS CONSULTAS -->
<!---->
<!--<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3"-->
<!--       style="border-bottom:none;border-top:none;">-->
<!--    <tr>-->
<!--        <td onclick="javascript:onOffBloco( 'minhasconsultas' );">-->
<!--            <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;-->
<!--            Minhas Consultas-->
<!--            <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0"/>-->
<!--        </td>-->
<!--    </tr>-->
<!--</table>-->
<!--<div id="minhasconsultas_div_filtros_off">-->
<!--</div>-->
<!--<div id="minhasconsultas_div_filtros_on" style="display:none;">-->
<!--    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">-->
<!--        <tr>-->
<!--            <td width="195" class="SubTituloDireita" valign="top">Consultas</td>-->
<!--            --><?php
//
//            $sql = sprintf(
//                "SELECT
//									CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
//																	   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
//																	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
//																 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
//																 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
//									END as acao,
//									'' || prtdsc || '' as descricao
//								 FROM
//								 	public.parametros_tela
//								 WHERE
//								 	mnuid = %d AND usucpf = '%s'",
//                $_SESSION['mnuid'],
//                $_SESSION['usucpf']
//            );
//
//            $cabecalho = array('Ação', 'Nome');
//            ?>
<!--            <td>-->
<!--                --><?php //$db->monta_lista_simples($sql, $cabecalho, 50, 50, 'N', '80%', null); ?>
<!--            </td>-->
<!--        </tr>-->
<!--    </table>-->
<!--</div>-->
<!-- FIM MINHAS CONSULTAS -->
<?php




        $funid1 = " in ('" . ACA_ID_UNIVERSIDADE . "')";
        $funid2 = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
        $funid3 = " in ('" . ACA_ID_UNIDADES_VINCULADAS . "')";




$stSqlCarregados = "";
$stSql = "SELECT
					e.entid AS codigo,
					e.entnome AS descricao
				FROM
					entidade.entidade e
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid



				WHERE
					e.entstatus = 'A' AND (ef.funid " . $funid1 . " OR ef.funid " . $funid2 . " OR ef.funid " . $funid3 . ")

				ORDER BY
					e.entnome";

//ver($stSql,d);


?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <?php
    // Unidades

    mostrarComboPopup('Unidades', 'unidade', $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)');

    $funid1 = ACA_ID_CAMPUS;
    $funid2 = ACA_ID_UNED;


    $stSql = "SELECT
					e.entid AS codigo,
					e.entnome AS descricao
        FROM
					entidade.entidade e2
				INNER JOIN
					entidade.entidade e ON e2.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ef.fueid = ea.fueid
				INNER JOIN
					academico.entidadeportaria ep ON ea.entid = ep.entid
				WHERE
					e.entstatus = 'A' AND (ef.funid = " . $funid1 . " OR ef.funid = " . $funid2 . ")

				ORDER BY
					e.entnome";

    mostrarComboPopup('Campus', 'campus', $stSql, $stSqlCarregados, 'Selecione o(s) Campus');

    // modulo
    $stSql = " SELECT
								mdlid AS codigo,
								mdldsc AS descricao
							FROM 
								academico.modulo
							ORDER BY
								mdldsc ";
    mostrarComboPopup('Módulo', 'modulos', $stSql, $stSqlCarregados, 'Selecione o(s) módulo(s)');

    // Ambiente
    $stSql = " SELECT
								ambid AS codigo,
								ambdsc AS descricao
							FROM 
								academico.ambiente
							ORDER BY
								ambdsc ";
    mostrarComboPopup('Ambiente', 'ambiente', $stSql, $stSqlCarregados, 'Selecione o(s) ambiente(s)');

    // Grupo de Município
    $stSql = "  SELECT
								undid as codigo,
								unddsc as descricao
							FROM
								academico.unidademed
							ORDER BY
								unddsc";
    mostrarComboPopup('Unidade de medida', 'unidademed', $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s) de medida');
    ?>
    <tr>
        <td class="SubTituloDireita">Percentual da Obra</td>
        <td>
            <table>
                <tr>
                    <th>Mínimo</th>
                    <th>Máximo</th>
                </tr>
                <tr>
                    <?php
                    $arPercentual[] = array('codigo' => '0', 'descricao' => '0 %');
                    $arPercentual[] = array('codigo' => '5', 'descricao' => '5 %');
                    $arPercentual[] = array('codigo' => '10', 'descricao' => '10 %');
                    $arPercentual[] = array('codigo' => '15', 'descricao' => '15 %');
                    $arPercentual[] = array('codigo' => '20', 'descricao' => '20 %');
                    $arPercentual[] = array('codigo' => '25', 'descricao' => '25 %');
                    $arPercentual[] = array('codigo' => '30', 'descricao' => '30 %');
                    $arPercentual[] = array('codigo' => '35', 'descricao' => '35 %');
                    $arPercentual[] = array('codigo' => '40', 'descricao' => '40 %');
                    $arPercentual[] = array('codigo' => '45', 'descricao' => '45 %');
                    $arPercentual[] = array('codigo' => '50', 'descricao' => '50 %');
                    $arPercentual[] = array('codigo' => '55', 'descricao' => '55 %');
                    $arPercentual[] = array('codigo' => '60', 'descricao' => '60 %');
                    $arPercentual[] = array('codigo' => '65', 'descricao' => '65 %');
                    $arPercentual[] = array('codigo' => '70', 'descricao' => '70 %');
                    $arPercentual[] = array('codigo' => '75', 'descricao' => '75 %');
                    $arPercentual[] = array('codigo' => '80', 'descricao' => '80 %');
                    $arPercentual[] = array('codigo' => '85', 'descricao' => '85 %');
                    $arPercentual[] = array('codigo' => '90', 'descricao' => '90 %');
                    $arPercentual[] = array('codigo' => '95', 'descricao' => '95 %');
                    $arPercentual[] = array('codigo' => '100', 'descricao' => '100 %');

                    $percentualinicial = 0;
                    $percentualfinal = 100;
                    echo '<td>';
                    $db->monta_combo("percentualinicial", $arPercentual, 'S', 'Selecione', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                    echo '</td><td>';
                    $db->monta_combo("percentualfinal", $arPercentual, 'S', 'Selecione', 'validarPercentual', '', '', '', 'N', 'percentualfinal');
                    echo '</td>';
                    ?>
                </tr>
            </table>
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Mobiliada</td>
        <td>
            <input type='radio' id='mobiliada' name='mobiliada' value='sim'/> Sim
            <input type='radio' id='mobiliada' name='mobiliada' value='nao'/> Não
            <input type='radio' id='mobiliada' name='mobiliada' value='todos' checked="checked"/> Todas
        </td>
    </tr>

    <tr>
        <td class="SubTituloDireita">Tipo de ensino</td>
        <td><?php
            $orgaos = $db->carregar("SELECT
            orgid, orgdesc
            FROM
            academico.orgao");

            $count = count($orgaos);
            for ($i = 0; $i < $count; $i++) {
            echo '<input type="checkbox" id="orgid" name="orgid['.$i.']" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';
            }?>
        </td>
    </tr>
    <tr>
        <td bgcolor="#CCCCCC"></td>
        <td bgcolor="#CCCCCC">
            <input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');"
                   style="cursor: pointer;"/>
            <input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeralXLS();"
                   style="cursor: pointer;"/>
<!--            <input type="button" value="Salvar Consulta" onclick="obras_exibeRelatorioGeral('salvar');"-->
<!--                   style="cursor: pointer;"/>-->
        </td>
    </tr>
</table>
</form>
<script>

    function mostraResponsavel(valor) {

        var tr = document.getElementById('trResponsavel');
        var resp = document.getElementById('responsavel');

        if (valor == 'nao') {
            resp.value = '';
            tr.style.display = 'none';
        } else {
            tr.style.display = 'table-row';
        }

    }

</script>
