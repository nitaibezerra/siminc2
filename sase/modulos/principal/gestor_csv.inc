<?php

include APPRAIZ . 'includes/cabecalho.inc';


global $db;
exit('teste');

// -- insercao secundaria:


$sql = "
INSERT INTO sase.territorio ( mesid, muncod, usucpf, terstatus )
VALUES 
    ( 58, '5101407', NULL, 'A' ),
    ( 58, '5102850', NULL, 'A' ),
    ( 58, '5103254', NULL, 'A' ),
    ( 58, '5103379', NULL, 'A' ),
    ( 58, '5105150', NULL, 'A' ),
    ( 58, '5105176', NULL, 'A' ),
    ( 58, '5107578', NULL, 'A' ),

    ( 59, '5100250', NULL, 'A' ),
    ( 59, '5100805', NULL, 'A' ),
    ( 59, '5102793', NULL, 'A' ),
    ( 59, '5103205', NULL, 'A' ),
    ( 59, '5104104', NULL, 'A' ),
    ( 59, '5105606', NULL, 'A' ),
    ( 59, '5106158', NULL, 'A' ),
    ( 59, '5106216', NULL, 'A' ),
    ( 59, '5108808', NULL, 'A' ),
    ( 59, '5108956', NULL, 'A' ),
    ( 59, '5106190', NULL, 'A' ),
    ( 59, '5106265', NULL, 'A' ),
    ( 59, '5106299', NULL, 'A' ),
    ( 59, '5106422', NULL, 'A' ),
    ( 59, '5108055', NULL, 'A' ),

    ( 60, '5100359', NULL, 'A' ),
    ( 60, '5101852', NULL, 'A' ),
    ( 60, '5102694', NULL, 'A' ),
    ( 60, '5103353', NULL, 'A' ),
    ( 60, '5105309', NULL, 'A' ),
    ( 60, '5106315', NULL, 'A' ),
    ( 60, '5106778', NULL, 'A' ),
    ( 60, '5107750', NULL, 'A' ),
    ( 60, '5107776', NULL, 'A' ),
    ( 60, '5107859', NULL, 'A' ),
    ( 60, '5107883', NULL, 'A' ),
    ( 60, '5108600', NULL, 'A' ),

    ( 61, '5100201', NULL, 'A' ),
    ( 61, '5101001', NULL, 'A' ),
    ( 61, '5101209', NULL, 'A' ),
    ( 61, '5101803', NULL, 'A' ),
    ( 61, '5102603', NULL, 'A' ),
    ( 61, '5102702', NULL, 'A' ),
    ( 61, '5103106', NULL, 'A' ),
    ( 61, '5103908', NULL, 'A' ),
    ( 61, '5106174', NULL, 'A' ),
    ( 61, '5106257', NULL, 'A' ),
    ( 61, '5106281', NULL, 'A' ),
    ( 61, '5106653', NULL, 'A' ),
    ( 61, '5106703', NULL, 'A' ),
    ( 61, '5107065', NULL, 'A' ),
    ( 61, '5107180', NULL, 'A' ),
    ( 61, '5107198', NULL, 'A' ),
    ( 61, '5108204', NULL, 'A' ),

    ( 62, '5100300', NULL, 'A' ),
    ( 62, '5100409', NULL, 'A' ),
    ( 62, '5100607', NULL, 'A' ),
    ( 62, '5102678', NULL, 'A' ),
    ( 62, '5103601', NULL, 'A' ),
    ( 62, '5103858', NULL, 'A' ),
    ( 62, '5104203', NULL, 'A' ),
    ( 62, '5104609', NULL, 'A' ),
    ( 62, '5104807', NULL, 'A' ),
    ( 62, '5105200', NULL, 'A' ),
    ( 62, '5106307', NULL, 'A' ),
    ( 62, '5106372', NULL, 'A' ),
    ( 62, '5107008', NULL, 'A' ),
    ( 62, '5107040', NULL, 'A' ),
    ( 62, '5107602', NULL, 'A' ),
    ( 62, '5107297', NULL, 'A' ),
    ( 62, '5107404', NULL, 'A' ),
    ( 62, '5107792', NULL, 'A' ),
    ( 62, '5108105', NULL, 'A' ),

    ( 63, '5100102', NULL, 'A' ),
    ( 63, '5101605', NULL, 'A' ),
    ( 63, '5103007', NULL, 'A' ),
    ( 63, '5103403', NULL, 'A' ),
    ( 63, '5104906', NULL, 'A' ),
    ( 63, '5105903', NULL, 'A' ),
    ( 63, '5106109', NULL, 'A' ),
    ( 63, '5106208', NULL, 'A' ),
    ( 63, '5106455', NULL, 'A' ),
    ( 63, '5106505', NULL, 'A' ),
    ( 63, '5107701', NULL, 'A' ),
    ( 63, '5107800', NULL, 'A' ),
    ( 63, '5108402', NULL, 'A' ),

    ( 64, '5101258', NULL, 'A' ),
    ( 64, '5102504', NULL, 'A' ),
    ( 64, '5102686', NULL, 'A' ),
    ( 64, '5103304', NULL, 'A' ),
    ( 64, '5103361', NULL, 'A' ),
    ( 64, '5103437', NULL, 'A' ),
    ( 64, '5103809', NULL, 'A' ),
    ( 64, '5103957', NULL, 'A' ),
    ( 64, '5104500', NULL, 'A' ),
    ( 64, '5105002', NULL, 'A' ),
    ( 64, '5105234', NULL, 'A' ),
    ( 64, '5105622', NULL, 'A' ),
    ( 64, '5106182', NULL, 'A' ),
    ( 64, '5106752', NULL, 'A' ),
    ( 64, '5106828', NULL, 'A' ),
    ( 64, '5107156', NULL, 'A' ),
    ( 64, '5107206', NULL, 'A' ),
    ( 64, '5107750', NULL, 'A' ),
    ( 64, '5107107', NULL, 'A' ),
    ( 64, '5107875', NULL, 'A' ),
    ( 64, '5108352', NULL, 'A' ),
    ( 64, '5105507', NULL, 'A' ),

    ( 65, '5101704', NULL, 'A' ),
    ( 65, '5101902', NULL, 'A' ),
    ( 65, '5102637', NULL, 'A' ),
    ( 65, '5103452', NULL, 'A' ),
    ( 65, '5106232', NULL, 'A' ),
    ( 65, '5106851', NULL, 'A' ),
    ( 65, '5107263', NULL, 'A' ),
    ( 65, '5107958', NULL, 'A' ),

    ( 66, '5100508', NULL, 'A' ),
    ( 66, '5101308', NULL, 'A' ),
    ( 66, '5103502', NULL, 'A' ),
    ( 66, '5106000', NULL, 'A' ),
    ( 66, '5107305', NULL, 'A' ),
    ( 66, '5108857', NULL, 'A' ),
    ( 66, '5108907', NULL, 'A' ),

    ( 67, '5105259', NULL, 'A' ),
    ( 67, '5106224', NULL, 'A' ),
    ( 67, '5107768', NULL, 'A' ),
    ( 67, '5107925', NULL, 'A' ),
    ( 67, '5108006', NULL, 'A' ),

    ( 68, '5105101', NULL, 'A' ),
    ( 68, '5106273', NULL, 'A' ),
    ( 68, '5106802', NULL, 'A' ),
    ( 68, '5107941', NULL, 'A' ),

    ( 69, '5103056', NULL, 'A' ),
    ( 69, '5103700', NULL, 'A' ),
    ( 69, '5104559', NULL, 'A' ),
    ( 69, '5105580', NULL, 'A' ),
    ( 69, '5106240', NULL, 'A' ),
    ( 69, '5107248', NULL, 'A' ),
    ( 69, '5107909', NULL, 'A' ),
    ( 69, '5108303', NULL, 'A' ),
    ( 69, '5108501', NULL, 'A' )
";
$db->executar( $sql );
$resultado = $db->commit();
dbg($resultado);

exit('fim');

// -- insercao primeira:

$row = 0;
$orgcod = null;
$dados = array();

$handle = fopen ("../../sase/modulos/principal/dados_territorios.csv","rw");
echo "<br/><br/><br/><br/><br/><br/>comeca:<br/>";
while( ($data = fgetcsv($handle, 1000, ",")) !== FALSE ) {
    $row++;
	if( $row == 1 ) continue;

	$linha = explode(';',$data[0]);

    $estuf = ( $linha[0] )? $linha[0] : $estuf ;
    $orgcod = ( $linha[1] )? $linha[1] : $orgcod ;
    $mescod = ( $linha[2] )? $linha[2] : $mescod ;


    // busca orgid
    $sql1 = " select orgid from sase.orgao where orgcod = '{$orgcod}' and estuf = '{$estuf}' ";
    // if( $row == 279 ) ver($sql1,d);
    $orgid = $db->pegaUm( $sql1 );
    // ver($orgid);

    // busca mesid
    $sql2 = " select mesid from sase.mesoregiao where orgid = '{$orgid}' and mescod = '{$mescod}' ";

    $dados['mesid'] = $db->pegaUm( $sql2 );
    $dados['muncod'] = $linha[3];
    // $dados['usucpf'] = null;
    $dados['terstatus'] = 'A';

    // ver($dados,$sql2);

    // ver($dados, $sql1, $sql2, $row);
    // echo $dados['muncod'].'<br/>';

    // $sql3 = " select count(*) as total from sase.territorio where muncod = '". $dados['muncod'] ."' ";
    // $total = $db->pegaUm($sql3);
    // if( $total > 0 ) continue;

    // obs.: na linha 280 deu erro por ser maior que o muncod
    // if($row == 280){ echo $dados['muncod'].'<br/>';continue; }
	// if( $row < 280 ) continue;
    // obs.: na linha 388 deu erro por ser maior que o muncod
    // if($row == 388){ echo $dados['muncod'].'<br/>';continue; }
	// if( $row < 388 ) continue;
	// obs.: na linha 403 deu erro por ser maior que o muncod
    // if($row == 403){ echo $dados['muncod'].'<br/>';continue; }
    // if( $row < 403 ) continue;

    if( $dados['muncod'] == 5100208 ){ echo "passou<br/>";continue; }//$dados['muncod'] = 5106208;
    // if( $dados['muncod'] == 5106208 ) continue;

    // obs.: na linha 405 deu erro por não ter mesid - linha de muncod '5100102'
    if( $row < 406 ) continue;
    if( $row > 417 ) continue;
    // obs.: a combinação de orgcod = 3 e mescod = 15 não tem mesid
    // if( $orgid != 3 && $mescod != 15 ) continue;
    // if( $row < 412 ) continue;
    
    // ver($data,$dados,$sql2, $row);
    // $sql = " insert into sase.territorio ( mesid, muncod, terstatus ) values ( '".( implode("','",$dados) )."' ) ";
    // echo $sql;
    // $db->executar( $sql );
    // echo "<br/>Resultado:  " . ( ( $db->commit() )?"<font style='font-size:20px;color:green'>":"<font style='font-size:20px;color:red'>" ) . " Sucesso/Insucesso</font> <br/><br/> ";
}
fclose ($handle);