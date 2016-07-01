<?php
if ($_POST['execucao']) {
    ini_set("memory_limit", "2048M");
    $true = true;
    include("resultNutrisus.inc");
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$vpflcod = pegaPerfil($_SESSION['usucpf']);

monta_titulo('Relatório Nutrisus', '&nbsp;');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
        <title>Relatório</title>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/prototype.js"></script>
        <script type="text/javascript"><!--
        function gerarRelatorio(tipo_relatorio) {
                var formulario = document.formulario;

                if (tipo_relatorio == 'html') {
                    document.getElementById('tipo_relatorio').value = 'html';
                } else {
                    document.getElementById('tipo_relatorio').value = 'xls';
                }

                selectAllOptions(formulario.estado);

                if (!formulario.elements['estado'].value) {
                    alert('Selecione pelo menos um estado!');
                    return false;
                }

                selectAllOptions(formulario.municipio);
                
                var janela = window.open('', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
                formulario.target = 'relatorio';
                formulario.submit();

                janela.focus();
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

--></script>
    </head>
    <body>
        <form name="formulario" id="formulario" action="" method="post">
            <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="">
            <input type="hidden" name="execucao" id="execucao" value="listar">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">

                <tr>
                    <td class="subtituloDireita">Exercício:</td>
                    <td>
                        <?php
                        $sql = "
                                    select distinct to_char(nutdatregistro, 'YYYY') as codigo, to_char(nutdatregistro, 'YYYY') as descricao from pse.nutrisus";

                        $ano = $_POST['ano'];
                        $db->monta_combo('ano', $sql, 'S', '', null, null, null, null, 'N', 'ano', null, '', $ano, '', null, null);
                        ?>
                    </td>
                </tr>
                <?php
                // Filtros do relatório
                if ($vpflcod == SECRETARIA_ESTADUAL) {
                    $stSql = "SELECT est.estuf AS codigo,
                                     estdescricao AS descricao
                              FROM territorios.estado est
                              INNER JOIN pse.usuarioresponsabilidade usu ON est.estuf = usu.estuf  and rpustatus = 'A'
                              WHERE usucpf = '{$_SESSION['usucpf']}' ";

                    $stSqlCarregados = "SELECT est.estuf AS codigo,
                                     estdescricao AS descricao
                              FROM territorios.estado est
                              INNER JOIN pse.usuarioresponsabilidade usu ON est.estuf = usu.estuf  and rpustatus = 'A'
                              WHERE usucpf = '{$_SESSION['usucpf']}'";
                } else if ($vpflcod == SECRETARIA_MUNICIPAL) {
                    $stSql = "SELECT est.estuf AS codigo,
                estdescricao AS descricao
FROM territorios.estado est
INNER JOIN territorios.municipio mun ON mun.estuf = est.estuf
INNER JOIN pse.usuarioresponsabilidade usu ON mun.muncod = usu.muncod and rpustatus = 'A'
WHERE usucpf = '{$_SESSION['usucpf']}' ";

                    $stSqlCarregados = "SELECT est.estuf AS codigo,
                estdescricao AS descricao
FROM territorios.estado est
INNER JOIN territorios.municipio mun ON mun.estuf = est.estuf
INNER JOIN pse.usuarioresponsabilidade usu ON mun.muncod = usu.muncod and rpustatus = 'A'
WHERE usucpf = '{$_SESSION['usucpf']}'";
                } else {

                    $stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
                    $stSqlCarregados = "";
                }
                mostrarComboPopup('Estado', 'estado', $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)');

                if ($vpflcod == SECRETARIA_ESTADUAL) {
                    $stSql = "SELECT mun.muncod AS codigo ,
                                      mun.estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
                               FROM territorios.municipio mun
                               INNER JOIN pse.usuarioresponsabilidade usu ON mun.estuf = usu.estuf and rpustatus = 'A'
                               WHERE usucpf = '{$_SESSION['usucpf']}' 
                               ORDER BY descricao";

                    $stSqlCarregados = "";
//                                 $stSqlCarregados = "SELECT mun.muncod AS codigo ,
//                                      mun.estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
//                               FROM territorios.municipio mun
//                               INNER JOIN pse.usuarioresponsabilidade usu ON mun.estuf = usu.estuf and rpustatus = 'A'
//                               WHERE usucpf = '{$_SESSION['usucpf']}' 
//                               ORDER BY descricao";
                } else if ($vpflcod == SECRETARIA_MUNICIPAL) {

                    $stSql = "SELECT mun.muncod AS codigo,
       mun.estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
FROM territorios.municipio mun
INNER JOIN pse.usuarioresponsabilidade usu ON mun.muncod = usu.muncod
WHERE usucpf = '{$_SESSION['usucpf']}'
  AND rpustatus = 'A'
ORDER BY descricao;";

                    $stSqlCarregados = "SELECT mun.muncod AS codigo,
       mun.estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
FROM territorios.municipio mun
INNER JOIN pse.usuarioresponsabilidade usu ON mun.muncod = usu.muncod
WHERE usucpf = '{$_SESSION['usucpf']}'
  AND rpustatus = 'A'
ORDER BY descricao;";
                } else {

                    $stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
                }
                $where = array(
                    array(
                        "codigo" => "estuf",
                        "descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
                        "numeric" => false
                    )
                );
                mostrarComboPopup('Município', 'municipio', $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where);

//                $anoCenso = ANO_CENSO;
//                
//                           $stSql = "  SELECT  ent.entid as codigo,
//                ent.entnome as descricao
//
//        FROM entidade.entidade ent
//
//        INNER JOIN pse.escolabasepse esc ON ent.entcodent = esc.idecodinep
//        INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
//        INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
//        INNER JOIN territorios.municipio m ON m.muncod = ende.muncod
//
//        WHERE esc.ideanocenso = {$anoCenso} AND ent.entstatus='A'
//
//        ORDER BY m.estuf,m.mundescricao,ent.entnome";
//                
//                           $stSqlCarregados = "";
//                
//                
//		mostrarComboPopup( 'Instituição', 'entid',  $stSql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)' ); 
                ?>

                <tr>
                    <td align="center" colspan="2">
                        <input type="button" name="GerarRelatorio" value="Gerar Relatório HTML" onclick="javascript:gerarRelatorio('html');"/>
                        <input type="button" name="GerarRelatorio" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('xls');"/>
                    </td>
                </tr>
            </table>
        </form>
    </body>
</html>
