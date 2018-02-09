<?php

    include 'cabecalho.php';
    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    global $db;

/* Cria tabela temporária para armazenar as informações atualizadas */
for ($i = 1; $i <= 6; $i++){
    /* Deleta a tabela temporária existentes para criar com os dados atualizados */
    $sql = "truncate table carga.contratos_ob";
    $db->executar($sql);
    
    $sql2 = "INSERT INTO carga.contratos_ob(numero_ob, numero_empenho, codigodofavorecido, it_no_credor, observacao, unidade, natureza, favorecido, fonteempenho, datatransacao, obsano, obscnpj, ptres, programa, acao, localizador, unidadeorcamentaria, valor)
        select  numero_ob, numero_empenho, codigodofavorecido, it_no_credor, observacao, unidade, natureza, favorecido, fonteempenho, datatransacao, obsano, obscnpj, ptres, programa, acao, localizador, unidadeorcamentaria, valor
        from    dblink
                (
                        'dbname=
                         hostaddr=
                         user=
                         password=c
                         port=5432',
                        'select distinct
                        substr(numerodaob, 12, 12) as numero_ob,
                        --Case when length(trim(codigodainscricao_a_1)) = 12 then trim(codigodainscricao_a_1) else trim(codigodainscricao_a_2) end as numero_empenho,
                        trim(codigodainscricao_a_{$i}) as numero_empenho,
                        codigodofavorecido::character varying(18) as codigodofavorecido,
                        it_no_credor::character varying(255) as it_no_credor,
                        observacao::text as observacao,
                        codigodaugdooperador as unidade,
                        substr(classificacao_a_{$i}::character varying(9), 2, 6) as natureza,
                        it_no_credor::character varying(255) as favorecido,
                        --fonte_recurso::character varying(10),
                        '''' as fonteempenho,
                        datadeemissao::character varying(10) as datatransacao,
                        ''2014'' as obsano,
                        codigodofavorecido::character varying(18) as obscnpj, '''' as ptres, '''' as programa, '''' as acao, '''' as localizador, '''' as unidadeorcamentaria,
                        valordatransacao_{$i} as valor
        from siafi2014.ob ob
        left join dw.credor c ON trim(c.it_co_credor) = trim(ob.codigodofavorecido)
        --left join dw.ptres p ON p.ptres = ob.ptres
        where length(trim(codigodofavorecido)) = 14 --and ( codigodainscricao_a_1 like ''%NE%'' or codigodainscricao_a_2 like ''%NE%'' )
                                       and codigodainscricao_a_{$i} like ''%NE%'' and length(trim(codigodainscricao_a_{$i})) = 12 and substr(numerodaob, 1, 6) in
        (''153047'', ''155007'', ''150229'', ''154357'', ''150246'', ''153104'',  ''150218'', ''153094'', ''150232'', ''154145'', ''158172'', ''150247'', ''150248'', ''153261'', ''150224'', ''254420'', ''154716'', ''150237'', ''153057'', ''154177'', ''154502'', ''150223'',
        ''152477'', ''151046'', ''153040'', ''153108'', ''154070'', ''150231'', ''153610'', ''150221'', ''153286'', ''153071'', ''155001'', ''155008'', ''154003'', ''153808'', ''153054'', ''150233'',  ''153038'', ''154039'', ''154106'', ''154072'', ''153079'',
        ''155009'', ''155013'', ''155014'', ''155016'', ''155017'', ''155019'', ''155020'', ''155021'', ''155022'', ''155023'', ''155124'' )
        '
              )as rs
                (
                                       numero_ob character varying(12),
                                       numero_empenho character varying(15),
                                       codigodofavorecido character varying(14),
                                       it_no_credor character varying(250),
                                       observacao text,
                                       unidade character varying(6),
                                       natureza character varying(8),
                                       favorecido character varying(250),
                                       fonteempenho character varying(10),
                                       datatransacao character varying(10),
                                       obsano character varying(4),
                                       obscnpj character varying(14),
                                       ptres character varying(6),
                                       programa character varying(4),
                                       acao character varying(4),
                                       localizador character varying(4),
                                       unidadeorcamentaria character varying(6),
                                       valor numeric
                        )
        where trim(unidade||numero_ob) not in ( select trim(unidade||ob) from contratos.ob_siafi where obsano = '2014');";


    $db->executar($sql2);

    for ($z = 1; $z <= 10; $z++){
        $sql3 = "DELETE FROM carga.contratos_ob where obsid in
                                (select obsid from ( select min(obsid) as obsid, obsano, numero_ob, unidade, count(1) from carga.contratos_ob group by obsano, numero_ob, unidade having count(1) > 1) as foo );";
        $db->executar($sql3);
    }


    $sql4="insert into contratos.ob_siafi ( ob , empenho , it_co_credor , it_no_credor , obsob , unidade , natureza ,  favorecido , fonteempenho , datatransacao , obsano , obscnpj , ptres , programa , acao ,
           localizador , unidadeorcamentaria, valor )
        select numero_ob, numero_empenho, codigodofavorecido, it_no_credor, observacao, unidade, natureza, favorecido, fonteempenho, datatransacao::date, obsano::integer, obscnpj, ptres, programa, acao, localizador, unidadeorcamentaria, valor
        from carga.contratos_ob where trim(unidade||numero_ob||numero_empenho) not in ( select trim(unidade||ob||empenho) from contratos.ob_siafi where obsano = '2014');";

    $db->executar($sql4);

    $db->commit();
}

$assunto   = "[SIG] Carregar OB do SIAFI";
$mailBody = "Fim do carregamento de OBs";

$mail = new PHPMailer();
$mail->IsSMTP();
$mail->SMTPDebug  = 0;
$mail->Debugoutput = 'html';
$mail->Host       = "";
$mail->Port       = 0;
$mail->SMTPAuth   = false;
$mail->From =  '';
$mail->FromName = "SIG - CONTRATOS";
$mail->AddAddress("henrique.couto@ebserh.gov.br");
$mail->AddAddress("irian.villalba@ebserh.gov.br");
$mail->AddAddress("gustavo.avila@ebserh.gov.br");

$mail->IsHTML(true);
$mail->Subject  = $assunto; // Assunto da mensagem
$mail->Body = html_entity_decode($mailBody); //Conteudo
$mail->Send();
