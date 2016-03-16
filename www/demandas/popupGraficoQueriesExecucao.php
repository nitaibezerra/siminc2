<?php

header("Content-Type: text/html; charset=ISO-8859-1",true);

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if(!$db->testa_superuser()){
    $db->close();
    die;
}

?>

<script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<h3 style="text-align: center; color: red;">Queries em execução</h3>
<?php
$sql = "----Verifica query's rodando------------------------------------------
        select datname, pid || '&nbsp;' as pid, '<span '||case when usename='postgres' then 'style=\"background-color:#DAA520;\"' else '' end||'>'||usename||'</span>' as usename,
                '<span title=\"' || query || '\"><img class=\"img_detalhe\" src=\"/imagens/mais.gif\" style=\"margin-right: 5px;\" />' || substring(query, 0, 100) ||  '</span>',
                waiting,
              client_addr, (now() - backend_start) as tempo_backend, (now() - query_start) as tempo_query,
              ((substring(replace((now() - query_start)::interval::varchar,':',''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,':',''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,':',''),5,9)::float)) dur_segundos
               , nome
        from pg_stat_activity
                left join seguranca.ip_interno ip on ip.ip = substr(client_addr::text, 0, strpos(client_addr::text, '/'))
        where --query_start >= '2012-12-03'::date and
        query not like  '%IDLE%'
        and state not ilike '%IDLE%'
        --and datname = 'dbpingifescoleta2013' - and query not like  '%VACUUM%' and query not like  '%ANALYZE%' --and waiting = 't'
        --and ( (substring(replace((now() - query_start)::interval::varchar,':',''),1,2)::integer*3600)+(substring(replace((now() - query_start)::interval::varchar,':',''),3,2)::integer*60)+ (substring(replace((now() - query_start)::interval::varchar,':',''),5,9)::float))  > 50
        order by tempo_query desc
";
//$dados = $db->carregar($sql);
$cabecalho = array('banco', 'Processo', 'User', 'Query', 'Waiting', 'IP', 'Tempo Backend', 'Tempo Query', 'Segundos', 'Responsável');
$db->monta_lista($sql, $cabecalho, 20000, 5, 'S', '95%', '', '', '', '', '');

$db->close();
?>
<script type="text/javascript">

    jQuery(function(){
        jQuery('body').on('click', '.img_detalhe', function(){
            if(jQuery(this).attr('src') == '/imagens/mais.gif'){
                var valorAntigo = jQuery(this).parent().html();
                var valorNovo = jQuery(this).parent().attr('title');
                jQuery(this).parent().attr('title', valorAntigo);
                jQuery(this).parent().html('<img class=\"img_detalhe\" src=\"/imagens/menos.gif\" style=\"margin-right: 5px;\" />' + valorNovo);

                jQuery(this).attr('src', '/imagens/menos.gif');
            } else {
                var valorAntigo = jQuery(this).parent().html();
                var valorNovo = jQuery(this).parent().attr('title');
                jQuery(this).parent().attr('title', valorAntigo);
                jQuery(this).parent().html(valorNovo);

                jQuery(this).attr('src', '/imagens/mais.gif');
            }

        });
    });        
</script>
