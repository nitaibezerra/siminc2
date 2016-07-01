<?php
# Inclui cabecalho
include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

# Recupera os dados do Municipio
if( $_SESSION['par']['itrid'] == '2' ) {
    # Dados do Municipio
    $muncod = $_SESSION['par']['muncod'];
    $sqlMunicipio = "SELECT estuf, mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}'";
    $arrMunicipio = reset($db->carregar($sqlMunicipio));
    $where = "i.muncod = '{$muncod}'";
}else{
    # Dados do Estado
    $estuf = $_SESSION['par']['estuf'];
    $sqlUf = "SELECT estuf, estdescricao FROM territorios.estado WHERE estuf = '{$estuf}'";
    $arrUf = reset($db->carregar($sqlUf));
    $where = "uf.estuf = '{$estuf}'";
}
?>
<!-- Titulo do Municipio -->
<table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" style="border-top: none; border-bottom: none;" class="tabela">
    <tbody>
        <tr>
            <td width="100%" align="center">
                <label style="color:#000000;" class="TituloTela">
                    <?php
                    if ($estuf) {
                        echo "{$arrUf['estuf']} - {$arrUf['estdescricao']}";
                    } else {
                        echo "{$arrMunicipio['estuf']} - {$arrMunicipio['mundescricao']}";
                    }
                    ?>
                </label>
            </td>
        </tr>
    </tbody>
</table>
<!-- /Titulo do Municipio -->

<!-- Titulo do Restricoes Municipio -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
    <tr>
        <td align="center" colspan="2"><b>Restrições para o <?php echo ($estuf) ? 'Estado' : 'Município' ?></b></td>
    </tr>
</table>
<!-- /Titulo do Restricoes Municipio -->
<?php
#Busca as Restricoes para o Municipio
if ($estuf) {
    $acao = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',\'' || uf.estuf || '\')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
    $cabecalho = array("ações", "Código", "Descrição", "Sigla", "UF", "Estado", "Tipo de Restrição");
    $campo = "uf.estuf, uf.estdescricao, ";
} else {
    $acao = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid || ',' || mun.muncod ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
    $cabecalho = array("ações", "Código", "Descrição", "UF", "Código Município", "Município", "Estado", "Tipo de Restrição");
    $campo = "i.mun_estuf, i.muncod, mun.mundescricao,";
}
$sql = "
        SELECT DISTINCT
            '<center>'||" . $acao . "||'</center>' AS acao,
            re.resid, 
            res.resdescricao, 
            $campo
            esd.esddsc,
            tpr.tprdescricao
        FROM par.restricaoentidade re 
        INNER JOIN par.restricaofnde res ON res.resid = re.resid 
        INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
        LEFT JOIN par.instrumentounidade i ON re.inuid = i.inuid
        LEFT JOIN territorios.municipio mun ON mun.muncod = i.muncod
        LEFT JOIN territorios.estado uf ON uf.estuf = i.estuf
        LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
        LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
        WHERE {$where} 
            AND res.resstatus = 'A' 
            AND res.resinformar = 'S'
            AND esd.esdid IN (".ESDID_EM_DILIGENCIA.",".ESDID_EM_TRATATIVA_PROVIDENCIA.")
        ORDER BY
            re.resid ASC
    ";

# Carrega lista de Restricoes por Municipio
$db->monta_lista($sql, $cabecalho, 25, 10, 'N', 'center', 'N');
?>

<!-- Titulo do Restricoes de Termo -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
    <tr>
        <td align="center" colspan="2"><b>Restrições para Termos</b></td>
    </tr>
</table>
<!-- Titulo do Restricoes de Termo -->
<?php
# Carrega lista de Restricoes por Termo para o Municipio
$acaoTermo = "'<img onclick=\"listaEntidade('|| re.resid || ',' || re.docid ||')\" style=\"cursor:pointer;\" src=\"../imagens/icone_lupa.png\">'";
$listaFiltro = array('muncod' => $muncod, 'viewMunicipio' => 'S', 'camposRestricao' => 'res.resid, res.resdescricao, esd.esddsc, ', 'botaoacao' => $acaoTermo, 'whereParametrizado' => "AND res.resinformar = 'S' AND esd.esdid IN (".ESDID_EM_DILIGENCIA.",".ESDID_EM_TRATATIVA_PROVIDENCIA.")");
$sqlRestricaoTermo = listarTermo($listaFiltro, 'SQL', false);

# Carrega lista de Restricoes por Municipio
$cabecalhoTermo = array("Ações", "Código", "Descrição", "Situação", "Termo", "Tipo do Termo", "Vigência", "Banco", "Agência", "Conta Corrente", "Valor Pago(R$)", "Processo", "Tipo do Processo");
$db->monta_lista($sqlRestricaoTermo, $cabecalhoTermo, 25, 10, 'N', 'center', 'N');
?>

<!--@TODO VErificar chamadas de Jquery-->
<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>

<script>
    function listaEntidade(resid, docid, muncod)
    {
        var title = 'Restrição detalhada #' + resid;
        var data = {action: 'visualizar', resid: resid, docid: docid, muncod: muncod, btnVoltar:'N'};
        var url = 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/popupEntidade&acao=A';

        dialogAjax(title, data, url, null, false, true, true, true, false, 900, 400);
    }
</script>

