<?
include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Controle de Erros", "");

$sql = "SELECT u.usucpf, u.usunome, u.usuemail, e.eeuid FROM seguranca.envioerrosusuarios e INNER JOIN seguranca.usuario u ON u.usucpf = e.usucpf WHERE eeustatus='A'";
$usus = $db->carregar($sql);


$sql = "-- Qtd distintos por dia
        select 'Erros únicos' as descricao, data as categoria, count(*) as valor
        from (
                select distinct errtipo, sisid, errarquivo, errlinha, to_char(errdata, 'YYYY-MM-DD') as data, count(*)
                from seguranca.erro
                group by errtipo, sisid, errarquivo, errlinha, data
                order by data, sisid
        ) as foo
        group by categoria, descricao";

$sql = "select *
        from (
                select 'Erros únicos' as descricao, data as categoria, count(*) as valor
                from (
                        select distinct errtipo, sisid, errarquivo, errlinha, to_char(errdata, 'YYYY-MM-DD') as data, count(*)
                        from seguranca.erro
                        group by errtipo, sisid, errarquivo, errlinha, data

                ) as foo
                group by descricao, categoria

                union

                select 'Erros totais' as descricao, to_char(errdata, 'YYYY-MM-DD') as data, count(*)
                from seguranca.erro
                group by descricao, data
        ) as total
        order by categoria, descricao";

$sql2 = "-- Qtd distintos por dia
            select  case
                        when errtipo = 'DB' then 'Banco de dados'
                        when errtipo = 'PR' then 'Programação'
                        when errtipo = 'QB' then 'Queda no banco'
                        when errtipo = 'WS' then 'WebService'
                        when errtipo = 'EN' then 'Encoding no banco'
                        when errtipo = 'PD' then 'Erro na Conexão'
                        when errtipo = 'DC' then 'Diretório Cheio'
                        when errtipo = 'AI' then 'Arquivo inexistente'
                        when errtipo = 'DV' then 'Diversos'
                        else errtipo
                    end as descricao,
                    count(*) as valor
        from (
                select distinct errtipo, sisid, errarquivo, errlinha, to_char(errdata, 'YYYY-MM-DD') as data, count(*)
                from seguranca.erro
                group by errtipo, sisid, errarquivo, errlinha, data
                order by data, sisid
        ) as foo
        group by descricao
        order by valor desc
        ";

$sql3 = "-- Qtd distintos por dia
        select 'Sistema' as descricao, sisabrev as categoria, count(*) as valor
        from (
                select distinct errtipo, s.sisabrev, e.sisid, errarquivo, errlinha, to_char(errdata, 'YYYY-MM-DD') as data, count(*)
                from seguranca.erro e
                    left join seguranca.sistema s on s.sisid = e.sisid
                group by errtipo, s.sisabrev, e.sisid, errarquivo, errlinha, data
                order by data, e.sisid
        ) as foo
        group by categoria, descricao
        order by valor desc
        ";

$grafico = new Grafico();
$grafico->setTitulo('Qtd por dia')->setTipo(Grafico::K_TIPO_LINHA)->gerarGrafico($sql);
$grafico->setTitulo('Qtd única por tipo')->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($sql2);
$grafico->setTitulo('Qtd única por módulo')->setTipo(Grafico::K_TIPO_BARRA)->setLabelX(array())->setHeight('500px')->gerarGrafico($sql3);


$sql = "select distinct sisabrev, e.sisid, errdescricao,
                case
                    when errtipo = 'DB' then 'Banco de dados'
                    when errtipo = 'PR' then 'Programação'
                    when errtipo = 'QB' then 'Queda no banco'
                    when errtipo = 'WS' then 'WebService'
                    when errtipo = 'EN' then 'Encoding no banco'
                    when errtipo = 'PD' then 'Erro na Conexão'
                    when errtipo = 'DC' then 'Diretório Cheio'
                    when errtipo = 'AI' then 'Arquivo inexistente'
                    when errtipo = 'DV' then 'Diversos'
                    else errtipo
                end as descricao,
                errarquivo, errlinha, count(*)
        from seguranca.erro e
                left join seguranca.sistema s on s.sisid = e.sisid
        where to_char(errdata, 'YYYY-MM-DD') = to_char(NOW(), 'YYYY-MM-DD')
        group by errdescricao, errtipo, sisabrev, e.sisid, errarquivo, errlinha, errdata
        order by sisabrev, errarquivo, errlinha
        ";

$dados = $db->carregar($sql);

$aDados = array();
if($dados && is_array($dados)){
    foreach($dados as $count => $dado){
        $chave = $dado['sisid'] . $dado['errlinha'] . $dado['errarquivo'];
        $qtd[$chave]++;
        $aDados[$chave] = $dado;
        $aDados[$chave]['errlinha'] = $dado['errlinha'] . '&nbsp;';
        $aDados[$chave]['count'] = $qtd[$chave] . '&nbsp;';
        unset($aDados[$chave]['sisid']);
    }
}
$aDados = array_values($aDados);

echo '<h3 style="margin-top: 20px; color: red; text-align: center;">Erros ÚNICOS de hoje (' . date('d/m/Y') . ')</h3>';
$cabecalho = array('Módulo', 'Erro', 'Tipo', 'Arquivo', 'Linha', 'Qtd.');
$db->monta_lista($aDados, $cabecalho, 50000000, 4, 'N','Center');
?>