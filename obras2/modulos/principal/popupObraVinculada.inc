<?php
verificaSessao('obra');
$obrid = $_SESSION['obras2']['obrid'];
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script src="/obras2/js/obras2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script type="text/javascript">
    function exibeBuscaAvancada(visivel) {
        if (visivel == true) {
            $('#tr_busca_avancada').show();
            $('#labelBuscaAvancada').hide();
            $('#abreBuscaAvancada').val('t');
        } else {
            $('#tr_busca_avancada').hide();
            $('#labelBuscaAvancada').show();
            $('#abreBuscaAvancada').val('f');
        }
    }

    function carregarMunicipio(estuf) {
        var td = $('#td_municipio');
        if (estuf != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'municipio', estuf: estuf},
                dataType: "html",
                async: false,
                beforeSend: function () {
                    divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function () {
                    divCarregado();
                },
                success: function (data) {
                    td.html(data);
                    divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true).attr('disabled', true);
        }
    }

    function abreEvolucaoFinan(obrid) {
        janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid=' + obrid, 800, 650);
    }

    function alterarObr(obrid) {
        location.href = '?modulo=principal/cadObra&acao=<?=$_GET['show']?>&obrid=' + obrid;
    }

    function novaObrVinculada(obrid) {
        location.href = '?modulo=principal/cadObra&acao=A&obridVinculado=' + obrid;
    }

    function excluirObr(obrid) {
        if (confirm('Deseja apagar este obra?')) {
            $('#obrid').val(obrid);
            $('#req').val('apagar');
            $('#formListaObra').submit();
        }
    }

    function abreDocumentoObjeto(obrid) {
        $('#req').val('abaDocumento');
        $('#obrid').val(obrid);

        $('#formListaObra').submit();
    }

    function abreRestricao(obrid) {
        $('#req').val('abaRestricao');
        $('#obrid').val(obrid);

        $('#formListaObra').submit();
    }

    function abreFotoObjeto(obrid) {
        janela('?modulo=principal/listaObras&acao=A&req=abaFotos&obrid=' + obrid, 800, 650);
    }
</script>

<?php
monta_titulo('Lista de Obras Vinculadas', '');


/**
 * Retorna a obra original
 * @param $obrid
 * @return mixed
 */
function pegaObraOriginal($obrid)
{
    global $db;
    $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obrid = {$obrid} AND obrstatus IN ('A')";
    $obra = $db->pegaLinha($strSQL);

    if($obra)
        return $obra['obrid'];
    else {
        $strSQL = "SELECT obrid, obridvinculado FROM obras2.obras WHERE obrid = {$obrid} AND obrstatus IN ('P')";
        $obra = $db->pegaLinha($strSQL);
        return pegaObraOriginal($obra['obridvinculado']);
    }
}

/**
 * Pega todas as obras vinculadas
 *
 * @param $obrid
 * @param array $vinculados
 * @return array
 */
function populaObrasVinculadas($obrid, $vinculados = array())
{
    global $db;
    $strSQL = "SELECT obrid FROM obras2.obras WHERE obridvinculado = {$obrid} AND obrstatus IN ('P')";
    $vinculada = $db->pegaUm($strSQL);

    if ($vinculada) {
        $vinculados[] = $vinculada;
        return populaObrasVinculadas($vinculada, $vinculados);
    }

    return $vinculados;
}


$strSQL = "SELECT obridvinculado FROM obras2.obras WHERE obrid = {$obrid}";
$obridvinculado = $db->pegaUm($strSQL);


$paramLista['managerOrder'] = array(
    13 => 'obrnome',
    17 => 'ocrdtinicioexecucao',
    18 => 'ocrdtterminoexecucao',
    20 => array('campo' => "DATE_PART('days', NOW() - o.obrdtultvistoria)::numeric", 'alias' => "obrdtultvistoria"),
    21 => 'obrdtultvistoria'
);

$obras = new Obras();
$cabecalho = $obras->cabecalhoListaSql();

$obridOriginal = pegaObraOriginal($obrid);
$obrasVinculadas = populaObrasVinculadas($obridOriginal);

$obrasVinculadas[] = $obridOriginal;


$db->monta_lista(listaSql($obrid, $obrasVinculadas), getCabecalho(), 100, 5, 'N', 'center', 'N', "formulario", null, null, null, $paramLista);
?>
<br/>
<center><input type="button" id="close_div_dialog_lista_vinculado2" class="close_div_dialog_lista_vinculado"
               value="Fechar"></center>




<?php


function getCabecalho(){
    return array('Ação', 'ID', 'ID Pré-Obra', 'Nº Processo', 'Ano Processo', 'Termo/Nº Convênio', 'Ano Termo/Convênio', 'Obra', 'Unidade Implantadora',
                    'Município', 'UF', 'Data de Inícioda Execução', 'Data de Términoda Execução', 'Situação da Obra', 'Última Vistoria Instituição',
                    '% Executado Instituição', 'Última Vistoria Empresa', '% Executado Empresa', 'Tipologia', 'Valor Contrato');
}

function listaSql($obrid, $obrasVinculadas)
{
    $coluns = array();
    $join = array();
    $where = array();


    $coluns['acao'] = "'<center><div>
                    <img
                            align=\"absmiddle\"
                            src=\"/imagens/alterar.gif\"
                            style=\"cursor: pointer\"
                            onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                            title=\"Alterar Obra\" />
              </div></center>' AS acao";


    $coluns['obrid'] = "o.obrid";
    $coluns['preid'] = "o.preid";
    $coluns['pronumeroprocesso'] = 'p_conv.pronumeroprocesso';
    $coluns['anoprocesso'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) anoprocesso";
    $coluns['termo_convenio'] = "p_conv.termo_convenio";
    $coluns['ano_termo_convenio'] = "p_conv.ano_termo_convenio";
    $coluns['obrnome'] = "
                        CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                            '<b style=\"color:green\">(EP) </b>'
                        ELSE
                            ''
                        END || '<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as obrnome";
    $coluns['entnome'] = "uni.entnome";
    $coluns['mundescricao'] = "mun.mundescricao";
    $coluns['estuf'] = "mun.estuf";
    $coluns['inicio'] = "TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY') AS inicio";
    $coluns['termino'] = "TO_CHAR(oc.ocrdtterminoexecucao, 'dd/mm/YYYY') AS termino";
    $coluns['strdsc'] = "
                    CASE
                        WHEN str.strid = " . STRID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || str.strdsc || '</font>'
                        ELSE str.strdsc
                    END AS situacao
                    ";
    $coluns['ultima_atualizacao'] = "
                    -- Obra concluida aplicar cor azul
                    CASE WHEN ed.esdid IN (693) THEN
                        '<span style=\"color:#3276B1\">' || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</span>'
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1)) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char((SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))||' dia(s) )' || '<span>'
                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        CASE WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) < 45 THEN
                            '<span style=\"color:#3CC03C\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#FAD200\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))) >= 60 THEN
                            '<span style=\"color:#FF0000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char((SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)), 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))||' dia(s) )' || '<span>'
                    ELSE
                        CASE WHEN DATE_PART('days', NOW() - obrdtultvistoria) < 45 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada em até 45 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 45 AND DATE_PART('days', NOW() - obrdtultvistoria) < 60 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                             WHEN DATE_PART('days', NOW() - obrdtultvistoria) >= 60 THEN
                            '<span style=\"color:#000000\" title=\"Esta obra foi atualizada a mais de 60 dias\">'
                        END || to_char(obrdtultvistoria, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - obrdtultvistoria)||' dia(s) )' || '<span>'
                    END as ultima_atualizacao";
    $coluns['obrpercentultvistoria'] = "o.obrpercentultvistoria || '%' as obrpercentultvistoria";
    $coluns['empdtultvistoriaempresa'] = "to_char(e.empdtultvistoriaempresa,'DD/MM/YYYY') as empdtultvistoriaempresa";
    $coluns['emppercentultvistoriaempresa'] = "e.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa";
    $coluns['tpodsc'] = "tpo.tpodsc";
    $coluns['ocrvalorexecucao'] = "ocrvalorexecucao";


    $sqlBase = "
        SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    " . (count($coluns) ? implode(", ", $coluns) : "") . "
                FROM obras2.obras o
                LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
                LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
                LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
                LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
                LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
                LEFT JOIN obras2.obrascontrato                    oc ON oc.ocrid = ocr.ocrid
                LEFT JOIN obras2.contrato c                          ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
                LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
                LEFT JOIN entidade.entidade uni                      ON uni.entid = e.entidunidade
                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid

                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = d.esdid
                LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid

                -- Dados do Processo e Termo
                LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

                LEFT JOIN (
                    SELECT
                            max( inc_estado ) as inc_estado,
                            obrid
                        FROM
                        (
                            SELECT DISTINCT
                                CASE
                                    WHEN rstdtsuperacao IS NOT NULL THEN 1
                                    WHEN rstdtprevisaoregularizacao > now() THEN 2
                                    WHEN rstdtprevisaoregularizacao < now() THEN 3
                                    ELSE 2
                                END as inc_estado,
                                obrid
                            FROM
                                obras2.restricao
                            WHERE
                                obrid IS NOT NULL AND
                rstitem = 'I' AND
                rststatus = 'A'
                            ) as  foo
                        GROUP BY
                            obrid
                        ) inc ON inc.obrid = o.obrid
                WHERE
                    o.obridpai IS NULL
                    AND o.obrstatus IN ('A', 'P')
                    AND
                    (
                        o.obrid IN ( ".implode(',', $obrasVinculadas)." )
                    )

                    -- AND o.obrid != " . $obrid . "

            ) as f
            ORDER BY f.obrid ASC
    ";

    return $sqlBase;
}

?>