<? if($prfid): ?>
<section class="panel panel-default">
    <section class="panel-heading" style="background-color:white;"><strong>GRÁFICO</strong></section>
    <section class="panel-body" style="background-color: #f5f5f5;">
    <?
    if($tipo == 'S'){
        $sql = <<<DML
            select
                COALESCE(esd.esddsc, 'Não Iniciado') as descricao,
                count(sa.sbacod) as valor
            FROM acomporc.acompanhamentosubacao sa
            LEFT JOIN workflow.documento doc ON doc.docid = sa.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE prfid = {$prfid}
            GROUP BY esd.esddsc
DML;

        $dados = $db->carregar($sql);
        if ($dados) {
            echo geraGrafico($dados, $nomeUnico = 'acoes', $titulo = 'Acompanhamento de Subações', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = true, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false, '#555555');
        }else{
            echo "<section class=\"alert alert-info text-center\">Não existem dados a serem apresentados</section>";
        }
    }else if($tipo == 'T'){
        $sql = <<<DML
            SELECT COALESCE(
                (SELECT esddsc
                FROM acomporc.relgdados rld
                INNER JOIN workflow.documento using(docid)
                INNER JOIN workflow.estadodocumento USING(esdid)
                WHERE rld.rlgid = rg.rlgid
                    AND rld.rldtipo IN('acao', 'acaorap')
                LIMIT 1
                ), 'Em preenchimento') AS descricao,
                count(acacod) as valor
            FROM acomporc.relgestao rg
            WHERE prfid = {$prfid}
            GROUP BY descricao
DML;
        $dados = $db->carregar($sql);
        if ($dados) {
            echo geraGrafico($dados, $nomeUnico = 'acoes', $titulo = 'Relatório de Gestão (TCU/CGU)', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = true, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false, '#555555');
        }else{
            echo "<section class=\"alert alert-info text-center\">Não existem dados a serem apresentados</section>";
        }
    }
    ?>

    </section>
</section>
<? endif; ?>