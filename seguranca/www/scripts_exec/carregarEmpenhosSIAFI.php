<?php

  include 'cabecalho.php';
  require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
  global $db;

/* Deleta a tabela temporária existentes para criar com os dados atualizados */
$sql = "truncate table carga.empenhos2014;";
$db->executar($sql);

/* Cria tabela temporária para armazenar as informações atualizadas */
$sql2 = "INSERT INTO carga.empenhos2014(nu_empenho, it_no_credor, natureza_despesa, observacao, ptres, fonte_recurso, codigo_ug_operador, data_transacao, valor_transacao, ano, programa, acao, unicod, localizador, codigo_favorecido, codigo_evento)
select  nu_empenho, it_no_credor, natureza_despesa, observacao, ptres, fonte_recurso, codigo_ug_operador, data_transacao, case when codigo_evento in ( '401091', '401092', '401096', '401097','401120', '401121', '401123', '401124' ) then valor_transacao else valor_transacao*-1 end as valor_transacao, ano, programa, acao, unicod, localizador, codigo_favorecido, codigo_evento
from    dblink
        (
                'dbname=
                 hostaddr=
                 user=
                 password=
                 port=',
                'select distinct
                substr(numero_ne, 12, 12) as nu_empenho,
                it_no_credor,
                natureza_despesa,
                observacao,
                ne.ptres,
                fonte_recurso,
                codigo_ug_operador,
                data_transacao,
                valor_transacao,
                ''2014'' as ano,
                substr(ptrprogramatrabalho, 6, 4) as programa,
                substr(ptrprogramatrabalho, 10, 4) as acao,
                p.unicod,
                substr(ptrprogramatrabalho, 14, 4) as localizador,
                codigo_favorecido,
                                        codigo_evento
from siafi2014.ne ne
left join dw.credor c ON c.it_co_credor = ne.codigo_favorecido
left join dw.ptres p ON p.ptres = ne.ptres
where codigo_ug_operador in
(''153047'', ''155007'', ''150229'', ''154357'', ''150246'', ''153104'',  ''150218'', ''153094'', ''150232'', ''154145'', ''158172'', ''150247'', ''150248'', ''153261'', ''150224'', ''254420'', ''154716'', ''150237'', ''153057'', ''154177'', ''154502'', ''150223'',
''152477'', ''151046'', ''153040'', ''153108'', ''154070'', ''150231'', ''153610'', ''150221'', ''153286'', ''153071'', ''155001'', ''155008'', ''154003'', ''153808'', ''153054'', ''150233'',  ''153038'', ''154039'', ''154106'', ''154072'', ''153079'',
''155009'', ''155013'', ''155014'', ''155016'', ''155017'', ''155019'', ''155020'', ''155021'', ''155022'', ''155023'', ''155124'' )
    '
      )as rs
        (
               nu_empenho text,
                it_no_credor character varying(55),
               natureza_despesa character varying(6),
                observacao character varying(300),
                ptres character varying(6),
                fonte_recurso character varying(10),
                codigo_ug_operador character varying(6),
                data_transacao date,
                valor_transacao numeric,
                ano text,
                programa character varying(4),
                acao character varying(4),
                unicod character varying(5),
                localizador character varying(4),
                codigo_favorecido character varying(14),
                codigo_evento character varying(6)
                )
where trim(codigo_ug_operador||nu_empenho) not in ( select trim(ungcod||nu_empenho) from contratos.empenho_siafi where ano = '2014');";

$db->executar($sql2);

for ($i = 1; $i <= 20; $i++){
	$sql3 = "delete from carga.empenhos2014 where epsid in
(select epsid from 
( select min(epsid) as epsid, ano, nu_empenho, codigo_ug_operador, count(*) from carga.empenhos2014 group by ano, nu_empenho, codigo_ug_operador having count(*) > 1) as foo );";
	$db->executar($sql3);
}

$sql4="insert into contratos.empenho_siafi ( nu_empenho, no_favorecido, natureza, observacao, ptres, fonte, ungcod, dataempenho, valor, ano, programa, acao, unicod, localizador, co_favorecido, codigo_evento )
select nu_empenho, it_no_credor, natureza_despesa, observacao, ptres, fonte_recurso, codigo_ug_operador, data_transacao, valor_transacao, ano, programa, acao, unicod, localizador, codigo_favorecido, codigo_evento
from carga.empenhos2014 where trim(codigo_ug_operador||nu_empenho) not in ( select trim(ungcod||nu_empenho) from contratos.empenho_siafi where ano = '2014' );
";
$db->executar($sql4);


$db->commit();

$assunto   = "[SIG] Carregar empenhos do SIAFI";
$mailBody = "Fim do carregamento de empenhos";

$mail = new PHPMailer();
$mail->IsSMTP();
$mail->SMTPDebug  = 0;
$mail->Debugoutput = 'html';
$mail->Host       = "";
$mail->Port       = 0;
$mail->SMTPAuth   = false;
$mail->From =  '';
$mail->FromName = "SIG - CONTRATOS";
$mail->AddAddress("alisson.dutra@ebserh.gov.br");
$mail->AddAddress("henrique.couto@ebserh.gov.br");
$mail->AddAddress("irian.villalba@ebserh.gov.br");
//$mail->AddAddress("gustavo.avila@ebserh.gov.br");

$mail->IsHTML(true);
$mail->Subject  = $assunto; // Assunto da mensagem
$mail->Body = html_entity_decode($mailBody); //Conteudo
$mail->Send();

