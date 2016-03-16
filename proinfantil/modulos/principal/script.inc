<?php

include APPRAIZ . "includes/cabecalho.inc";
    include_once APPRAIZ . "includes/workflow.php";   
echo "<br />";
monta_titulo("E.I. Manutenção", "Selecione o módulo desejado:");
$_SESSION['usucpf'] = '';
unset($_SESSION['proinfantil']);

//$sql = "select turid, docid, turdsc, tirdescricao, tipoatendimento, entcodent, turdtinicio, dtanalise, tipoanalise, anaid, anatipo, lotdsc, anaparecer, esdid, esddsc, muncod, anaanalisado 
//from( SELECT distinct t.turdsc, tr.tirdescricao, t.turid, n.anaanalisado, ntw.docid, CASE WHEN t.ttuid = 1 THEN 'Exclusivo Creche' WHEN t.ttuid = 2 THEN 'Exclusivo Pré-escola' 
//WHEN t.ttuid = 3 THEN 'Creche e Pré-escola (Misto)' END as tipoatendimento, 
//t.entcodent, lt.lotdsc, t.turdtinicio, doc.esdid, esd.esddsc, 
//(select to_char(min(h.htddata), 'YYYY-MM-DD')
// from workflow.historicodocumento h 
// inner join workflow.acaoestadodoc ad on ad.aedid = h.aedid 
// where ad.esdidorigem = 535
//  and ad.esdiddestino = 536 
//  and h.docid = ntw.docid ) as dtanalise, 
// case when n.anatipo = 'A' then 'Aprovado' when n.anatipo = 'I' then 'Indeferida' 
// when n.anatipo = 'D' then 'Diligenciada' else 'Não Analisado' end as tipoanalise, n.anaid, n.anatipo, n.anaparecer, t.muncod 
// FROM proinfantil.turma t 
// inner join proinfantil.tiporede tr ON tr.tirid = t.tirid 
// inner join proinfantil.novasturmasworkflowturma ntw on ntw.turid = t.turid 
// inner join workflow.documento doc on doc.docid = ntw.docid 
// inner join workflow.estadodocumento esd on esd.esdid = doc.esdid 
// left join proinfantil.lotenovasturmas lt on lt.lotid = t.lotid and lt.lotstatus = 'A' 
// left join proinfantil.analisenovasturmasaprovacao n ON n.turid = t.turid and n.anastatus = 'A' 
// --WHERE t.muncod in ('3124203','3168606','5107925','4115309','4117701')
//WHERE t.muncod in ('3124203','3168606','5107925','4115309','4117701')
// and t.turstatus = 'A' 
// and t.turano = '2015' 
// and doc.esdid != 535
// --and tr.tirid = '2'
//  ORDER BY tr.tirdescricao ) 
// as foo WHERE true 
// --and anaparecer like '%Prezado Prefeito, alguns%'
// --and dtanalise <= '2015-06-16' 
//  AND tipoanalise in ( 'Não Analisado', 'Diligenciada') 
//  and esddsc != 'Em diligência'
// and esdid  in (1073) 
// AND CASE WHEN tipoanalise = 'Diligenciada' THEN (anaanalisado = false OR anaanalisado is null) ELSE true END;
//";
//
//$turmas = $db->carregar($sql);
//$count = '0';
//$lista;
//
//if($turmas){
//	foreach($turmas as $turma){
        wf_alterarEstado('40285469', '3700', '' , array());
       $count++;

//        $sql = "INSERT INTO proinfantil.analisenovasturmasaprovacao ( turid, anatipo, anaparecer, anastatus, usucpf, anadatainclusao, anaanalisado)
//            VALUES ($turma[turid],'D','Prezado Prefeito, alguns dados do Censo Escolar 2014 foram alterados no Simec. Por isso, estamos devolvendo em diligência para que possa conferir/corrigir de acordo com os novos dados e devolver para análise o quanto antes. Caso tenha alguma dúvida, entre em contato pelo e-mail: claudia.sousa@mec.gov.br',
//                'A','',NOW(), true)";
//
//         $resultado = $db->executar($sql);
//         $db->commit();
//  
//        }
//}
var_dump($count);
ver($count,d);
        
?>
