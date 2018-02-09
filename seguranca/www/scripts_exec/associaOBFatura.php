<?php

    include 'cabecalho.php';
    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    global $db;
   
   
    $assunto   = "[SIG] Associar OB ao Contrato";
    $mailBody = "Incio da associaчуo de OBs";

    $mail = new PHPMailer();
    $mail->IsSMTP();
    $mail->SMTPDebug  = 0;
    $mail->Debugoutput = 'html';
    $mail->Host       = "172.17.61.41";
    $mail->Port       = 25;
    $mail->SMTPAuth   = false;
    $mail->From =  '';
    $mail->FromName = "SIG - CONTRATOS";
    $mail->AddAddress("alisson.dutra@ebserh.gov.br");
    $mail->AddAddress("gustavo.avila@ebserh.gov.br");
    $mail->AddAddress("irian.villalba@ebserh.gov.br");
    $mail->IsHTML(true);
    $mail->Subject  = $assunto; // Assunto da mensagem
    $mail->Body = html_entity_decode($mailBody); //Conteudo
    $mail->Send();

    $sql = "select ctrid,
              obsob,
              ob,
              obs.valor ,
              now() ,
              true ,
              ungcod ,
              unicod ,
              datatransacao,
              epsid
             from contratos.ctcontrato ctr
            JOIN contratos.empenhovinculocontrato USING (ctrid)
            JOIN contratos.empenho_siafi emp using (epsid)
            JOIN contratos.ob_siafi obs ON (emp.nu_empenho = obs.empenho and emp.co_favorecido = obs.obscnpj)
            WHERE obsob like 'NF%'";

    $rs = $db->carregar($sql);

    foreach($rs as $reg) {
        $obs = explode(' ', $reg['obsob']);
        $nf = substr($obs[0], 2, 10);
        if (is_numeric($nf)) {
            $ctrid = $reg['ctrid'];
            $ftcnumero = $nf;
            $ob = $reg['ob'];

            $sql = "select ftcid, ftcnumero  from contratos.faturacontrato ftc
                    where ctrid = $ctrid and ftcnumero::integer = $ftcnumero
                    and ftcid not in
                    (select ftcid from contratos.ordembancariafatura obf
                    where obfnumero=trim('$ob') and obf.ftcid=ftc.ftcid)";

            $rsNF = $db->carregar($sql);

            if (count($rsNF) > 0) {
                $ftcid = $nf;
                $obfnumero = $reg['ob'];
                $obfvalor = $reg['valor'];
                $usucpf = $_SESSION['usucpf'];
                $obfdata = date("Y-m-d");
                $obfsiafi = true;
                $ungcod = $reg['ungcod'];
                $unicod = $reg['unicod'];
                $obfdatatransacao = date("Y-m-d");
                $epsid = $reg['epsid'];
                $obfsiafi = ( $obfsiafi)? "'t'" : "'f'";

                $sql =  "INSERT INTO contratos.ordembancariafatura(ftcid, obfnumero, obfvalor, usucpf, obfdata, obfsiafi,ungcod, unicod, obfdatatransacao, epsid) 
                         VALUES ( $ftcid, '$obfnumero', '$obfvalor', '$usucpf', '$obfdata', $obfsiafi, $ungcod, $unicod, '$obfdatatransacao', $epsid)";
                $db->executar($sql);

            }

        }
    }

$assunto   = "[SIG] Associar OB ao Contrato";
$mailBody = "Fim da associaчуo de OB aos contratos";

    $mail = new PHPMailer();
    $mail->IsSMTP();
    $mail->SMTPDebug  = 0;
    $mail->Debugoutput = 'html';
    $mail->Host       = "172.17.61.41";
    $mail->Port       = 25;
    $mail->SMTPAuth   = false;
    $mail->From =  '';
    $mail->FromName = "SIG - CONTRATOS";
    $mail->AddAddress("alisson.dutra@ebserh.gov.br");
    $mail->AddAddress("gustavo.avila@ebserh.gov.br");
    $mail->AddAddress("irian.villalba@ebserh.gov.br");
    $mail->IsHTML(true);
    $mail->Subject  = $assunto; // Assunto da mensagem
    $mail->Body = html_entity_decode($mailBody); //Conteudo
    $mail->Send();

?>