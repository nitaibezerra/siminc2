<?
/*
  Sistema Simec
  Setor responsvel: MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
  Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
  Mdulo:inicio.inc
  Finalidade: permitir abrir a página inicial
 */
//Chamada de programa
include_once '_funcoes.php';
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
include APPRAIZ . "includes/cabecalho.inc";

$redePerfil = 'MEC';

/* Retornando a Rede Ofertante */
$sqlOferntatne = <<<QUERY
SELECT DISTINCT
    co_rede_ofertante as codigo,
    ds_rede_ofertante as descricao
FROM
    pronatec.ofertaresumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_rede_ofertante
QUERY;

/* Retornando a Situação da Oferta */
$sqlStatusOferta = <<<QUERY
SELECT DISTINCT
    co_status_oferta_turma as codigo,
    ds_status_oferta_turma as descricao
FROM
    pronatec.ofertaresumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_status_oferta_turma
QUERY;

/* Retornando a Situação da Oferta */
$sqlEixoTecnologico = <<<QUERY
SELECT DISTINCT
    co_eixo_tecnologico as codigo,
    ds_eixo_tecnologico as descricao
FROM
    pronatec.ofertaresumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_eixo_tecnologico
QUERY;

/* Retornando a Situação da Oferta */
$sqlTipoCursos = <<<QUERY
SELECT DISTINCT
    co_tipo_curso AS codigo,
    ds_tipo_curso AS descricao
FROM
    pronatec.matricularesumo
WHERE ano_inicio_curso = '{$_SESSION['exercicio']}'
ORDER BY
    ds_tipo_curso
QUERY;

global $db;
$dataDadosAtualizados = $db->pegaUm("SELECT to_char(dt_materializacao,'dd/mm/yyyy') FROM pronatec.ofertaresumo LIMIT 1");
monta_titulo( "Bolsa Formação", "Ofertas de Turma");
?>
<style>
    .linkTabela {
        color: #008000;
        cursor: pointer;
        float: left;
        font-size: 17px;
        font-weight: 900;
        width: 20px;
    }
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
    function filtrarResultados(uf, municipio) {
        $('#filtros').hide();
        $('#voltarFiltros').show('fast');
        selectAllOptions(document.getElementById('rede_ofertante'));
        selectAllOptions(document.getElementById('status_oferta'));
        selectAllOptions(document.getElementById('eixo_tecnologico'));
        selectAllOptions(document.getElementById('tipo_curso'));
        
        var filtroEstado = (uf != '') ? '&sg_uf=' + uf : '';
        var filtroMunicipio = (municipio != '') ? '&co_municipio=' + municipio : '';
        
        ajaxatualizar('requisicao=filtrar&tipoResultado=oferta&' + $('#frmPronatec').serialize() + filtroEstado + filtroMunicipio, 'divResultado');
        $('#divResultado').show();
    }
    function voltarFiltros() {
        $('#filtros').show();
        $('#voltarFiltros').hide('fast');
        $('#divResultado').hide('fast');
    }

    function ajaxatualizar(params, iddestinatario) {
        document.getElementById(iddestinatario).innerHTML = "<br/><br/><br/><center><b>Carregando...<b></center>";
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: params,
            async: false,
            success: function(html) {
                if (iddestinatario != '') {
                    document.getElementById(iddestinatario).innerHTML = html;
                }
            }
        });
    }
    function detalharOfertaPorEstado(uf) {
        filtrarResultados(uf);
        $('#voltarOfertaPorMunicipio').remove();
        voltar = '<div id="divVoltarEstado" class="linkTabela" title="Voltar" onclick="voltarOfertaPorEstado()"> < </div>';
        $('.listagem').before(voltar);
    }
    function voltarOfertaPorEstado() {
        filtrarResultados();
    }
    function detalharOfertaPorMunicipio(uf, municipio) {
        filtrarResultados('', municipio);
        $('#divVoltarEstado').remove();
        voltar = '<div id="divVoltarMunicipio" class="linkTabela" title="Voltar" onclick="voltarOfertaPorMunicipio(\'' + uf + '\')"> < </div>';
        $('.listagem').before(voltar);
    }
    function voltarOfertaPorMunicipio(uf) {
         detalharOfertaPorEstado(uf);
    }
</script>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td>
            <div style="float: left; "> <b>Acessando perfil: <? echo $redePerfil; ?> </b></div>
            <div style="float: right;"> Dados atualizados em: <b><? echo $dataDadosAtualizados; ?> </b></div>
            <div id="voltarFiltros" style="clear: both; display: none;"> <input type="button" class="botao" value="Modificar Filtros" onclick="voltarFiltros();"/> </div>

        </td>
    </tr>
</table>
<form id="frmPronatec" action="?modulo=inicio&acao=C&filtro=true" >
    <table id="filtros" align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
        <tr>
            <td colspan="8" class="subTituloCentro">
                Filtros
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda" width="100px;">
                Rede Ofertante:
            </td>
            <td>
                <? echo combo_popup('rede_ofertante', $sqlOferntatne, 'Rede Ofertante', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>

            <td class="subtituloEsquerda" width="100px;">
                Situação da Turma:
            </td>
            <td>
                <? echo combo_popup('status_oferta', $sqlStatusOferta, 'Situação da Turma', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td class="subtituloEsquerda" width="100px;">
                Eixo Tecnológico:
            </td>
            <td>
                <? echo combo_popup('eixo_tecnologico', $sqlEixoTecnologico, ' Eixo Tecnológico', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
            <td class="subtituloEsquerda" width="100px;">
                Tipo de Curso:
            </td>
            <td>
                <? echo combo_popup('tipo_curso', $sqlTipoCursos, 'Tipo Curso', '300x400', 100, '', null, 'S', null, null, null, 200, false); ?>
            </td>
        </tr>
        <tr>
            <td colspan="8">
        <br/><center><input  type="button" style="width: 100px;" value="Filtrar" onclick="filtrarResultados();"/></center>
        </td>
        </tr>
    </table>
</form>
<br/>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0">
    <tr>
        <td class="subTituloCentro">
            Dados para <?php echo $_SESSION['exercicio']; ?>
        </td>
    </tr>
    <tr>
        <td>
            <div id="divResultado" ></div>
        </td>
    </tr>
</table>