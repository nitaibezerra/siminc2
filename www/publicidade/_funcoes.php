<?php

/**
 * Recupera o titulo da tela das tabelas de apoio 
 * @param int $abacod - codigo da aba
 * @param string $url - url da tela
 * @return string
 */
function consultarTituloTela($abacod, $url) {

    global $db;

    $sql = "select m.mnudsc
	              	from seguranca.menu m
	            where
	            	m.mnulink = '$url'";

    return $db->pegaUm($sql);
}

/**
 * Exibe mensagem de alerta no sistema 
 * @param string $mensagem - Mensagem para ser exibida
 */
function alerta($mensagem) {

    if (!empty($mensagem))
        echo "<script type='text/javascript'>alert('{$mensagem}')</script>";
}

/**
 * MÃ©todo responsÃ¡vel por redirecionar para pÃ¡gina solicitada e exibir uma mensagem passada como parÃ¢metro
 *
 * @name direcionar
 * @author 
 * @access public
 * @return mensagem do sucesso ou fracasso
 */
function direcionar($url, $msg = null) {
    if ($msg) {
        echo "<script>
	                alert('$msg');
	                window.location='$url';
	              </script>";
    } else {
        echo "<script>
	                window.location='$url';
	              </script>";
    }
    exit;
}

/**
 * MÃ©todo responsÃ¡vel por executar scripts da tela pai partindo da popup
 *
 * @name executarScriptPai
 * @author CÃ©zar Cirqueira
 * @access public
 * @return 
 */
function executarScriptPai($funcao) {
    echo "	<script>
					executarScriptPai('$funcao');
				</script>";
}

/**
 * MÃ©todo responsÃ¡vel por fechar popups
 *
 * @name fecharPopup
 * @author CÃ©zar Cirqueira
 * @access public
 * @return 
 */
function fecharPopup() {
    echo "	<script>
					self.close();
				</script>";
}

/**
 * Formata o valor numeric para ser inserido no banco 
 * @name formata_valor_sql
 * @author Silas Matheus
 * @access public
 * @return float
 */
function formata_valor_sql($valor) {

    $valor = str_replace('.', '', $valor);
    $valor = str_replace(',', '.', $valor);

    return $valor;
}

function listaGeral($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos com Publicidade de Utilidade Pública</b><br><br><table class="tabela" style="width:100% !important; font-size:1px !important;" align="center" border="1">';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1" >';
    }

    $sql = "select distinct ite.ipanumitempad, 
padnumsidoc,
camtitulo,
orgdsc,
ite.ipadsc, 
catdsc, 
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as ipavaloritem, 
fip.fipnumfaturafornecedor, 
fip.fipnumfaturaagencia,
case WHEN  pip.pipstatus = 'I' THEN '' ELSE pip.pipnumordembancaria END as pipnumordembancaria 
	    FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid
AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid
AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P')";

    if (!empty($dados['forid'])) {

        $sql .= "and ctt.forid =" . $dados['forid'];
    }


    if (!empty($dados['ano'])) {
        $sql .= " and pad.padano =" . $dados['ano'];
    }

    if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
        $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
    }

    $sql .= " order by ipanumitempad asc";
    //ver($sql,d);
    $registros = $db->carregar($sql);

    $total = 0;
    if (!empty($registros)) {
        $html .=
                '<tr>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 80px">Categoria</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>
    <!--                <td style="text-align: left;font-weight: bold;width: 300px">Agência</td>
            <td style="text-align: left;font-weight: bold">Situação</td>-->
        </tr>';

        foreach ($registros as $value) {
            $total += $value[ipavaloritem];
            $html .=
                    '<tr>
                <td style="text-align: center;font-size:9px;">' . $value[ipanumitempad] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[padnumsidoc] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[camtitulo] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[orgdsc] . '</td>
                <td style="text-align: left;font-size:9px;">' . $value[ipadsc] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[catdsc] . '</td>
                <td style="text-align: right;font-size:9px;">' . number_format($value[ipavaloritem], 2, ',', '.') . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[fipnumfaturafornecedor] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[fipnumfaturaagencia] . '</td>
                <td style="text-align: center;font-size:9px;">' . $value[pipnumordembancaria] . '</td>
            </tr>';
        }

        $html .='<tr><td colspan="10" style="text-align: center;background-color: #cccccc;font-size:9px;"><b>Total Geral: R$' . number_format($total, 2, ',', '.') . ' </b></td></tr>';
    }
    $html .= '</table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table></body>';
    }

    return $html;
}

function listaSolicitante($dados, $tipoRelatorio) {
    global $db;

    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Solicitante</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1">';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center"  border="1" >';
    }

    $sql = "select * from publicidade.vworgao";
    if (!empty($dados['orgao'])) {
        $sql .= " where orgid = '" . $dados['orgao'] . "'";
    }
    $sql .= " order by orgdsc";

    $campanhas = $db->carregar($sql);
    $total_geral = 0;
    foreach ($campanhas as $key) {

        $sql = "select ipanumitempad,
            padnumsidoc, 
            camtitulo,
            (select forn.fornome from publicidade.fornecedor forn where ite.forid = forn.forid) as agencia, 
            ipadsc, 
            catdsc,
            case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric  
            when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as ipavaloritem, 
            fip.fipnumfaturafornecedor, 
            fip.fipnumfaturaagencia,
            case WHEN  pip.pipstatus = 'I' THEN '' ELSE pip.pipnumordembancaria END as pipnumordembancaria 
	      FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid
AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid
AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid";

        $sql .= " where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') and org.orgid ='" . $key[orgid] . "'";

        if (!empty($dados['forid'])) {
            $sql .= " and ctt.forid =" . $dados['forid'];
        }
        if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }

        if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
            $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
        }
        
        $registros = $db->carregar($sql);

        $total = 0;
        if (!empty($registros)) {
            $html .=

                    '<tr>
                <td style="text-align: center;font-size: 16px;padding: 10px 10px 10px 0" colspan="10">' . $key[orgdsc] . '</td>
            </tr>
            <tr >

                <td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
                <td style="font-size:9px;text-align: center;font-weight: bold; width: 150px">Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold; width: 1200px">Descrição</td>
                <td style="font-size:9px;text-align: center;font-weight: bold; width: 80px">Categoria</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>
            </tr>';

            foreach ($registros as $value) {
                $total += $value[ipavaloritem];
                $total_geral += $value[ipavaloritem];
                $html .=
                        '<tr>
                    <td style="font-size:9px;text-align: center">' . $value[ipanumitempad] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[padnumsidoc] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[camtitulo] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[agencia] . '</td>
                    <td style="font-size:9px;text-align: left">' . $value[ipadsc] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[catdsc] . '</td>
                    <td style="font-size:9px;text-align: right">' . number_format($value[ipavaloritem], 2, ',', '.') . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturafornecedor] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturaagencia] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[pipnumordembancaria] . '</td>
                </tr>';
            }
            $html .=
                    '<tr><td colspan="10" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($total, 2, ',', '.') . '</b></td> </tr>';
        }
    };
    $html .=
            '<tr><td colspan="10" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total Geral: R$ ' . number_format($total_geral, 2, ',', '.') . '</b></td> </tr>
    </table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html .= '</body>';
    }
    return $html;
}

function listaFornecedor($dados, $tipoRelatorio) {
    global $db;

    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Fornecedor</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1">';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1">';
    }

    $sql = "select fornome, forid from publicidade.fornecedor ";
    if (!empty($_REQUEST['forid'])) {
        $sql .= " where forid = " . $_REQUEST['forid'];
    }
    $sql .= " order by fornome ";
    $fornecedor = $db->carregar($sql);
    $total_geral = 0;
    foreach ($fornecedor as $key) {

        $sql = "select fornome as fornecedor,orgdsc,padnumsidoc,ipanumitempad,ipadsc,
            case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
            when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as ipavaloritem, 
            fipnumfaturafornecedor, fipnumfaturaagencia,
            case WHEN  pip.pipstatus = 'I' THEN '' ELSE pipnumordembancaria END as pipnumordembancaria 
	      FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid";

        $sql .= " where ite.forid =" . $key[forid];

        if (!empty($dados['forid'])) {
            $sql .= " and ite.forid =" . $dados['forid'];
        }

        if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }

        if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
            $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
        }
        $sql .= "  and pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') order by fornome";
        $registros = $db->carregar($sql);
        $total = 0;
        if (!empty($registros)) {
            $html .=

                    '<tr>
                <td style="text-align: center;font-size: 16px;padding: 10px 10px 10px 0" colspan="9">' . $key[fornome] . '</td>
            </tr>
            <tr >

                <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>
            </tr>';

            foreach ($registros as $value) {
                $total += $value[ipavaloritem];
                $total_geral += $value[ipavaloritem];
                $html .=
                        '<tr>
                    <td style="font-size:9px;text-align: center">' . $value[fornecedor] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[orgdsc] . '</td>  
                    <td style="font-size:9px;text-align: center">' . $value[padnumsidoc] . '</td>   
                    <td style="font-size:9px;text-align: center">' . $value[ipanumitempad] . '</td>                    
                    <td style="font-size:9px;text-align: left">' . $value[ipadsc] . '</td>
                    <td style="font-size:9px;text-align: right">' . number_format($value[ipavaloritem], 2, ',', '.') . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturafornecedor] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturaagencia] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[pipnumordembancaria] . '</td>
                </tr>';
            }
            $html .=
                    '<tr><td style="font-size:9px;background-color: #cccccc;text-align: center" colspan="9"><b>Total: R$ ' . number_format($total, 2, ',', '.') . '</b></td> </tr>';
        }
    };

    $html .='<tr><td style="font-size:9px;background-color: #cccccc;text-align: center" colspan="9"><b>Total Geral: R$ ' . number_format($total_geral, 2, ',', '.') . '</b></td> </tr>
    </table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html .= '</body>';
    }

    return $html;
}

function listaCategoriaDetalhada($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Categoria Analítico</b><br><br>
                   <table class="tabela" style="width:100% !important;" align="center" border="1">';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1">';
    }

    $sql = "select distinct cat.catdsc, cat.catid
	      FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid";

    if (!empty($dados['categoria'])) {

        $sql .= " where catid =" . $dados['categoria'];
    }
    
     if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }


    $sql .= " and pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') order by catdsc";

    $categorias = $db->carregar($sql);

    foreach ($categorias as $key) {

        $sql = "select fornome as fornecedor,orgdsc, padnumsidoc, ipanumitempad,ipadsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric when coalesce(fip.fipvalortotal,0) > 0 
then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as ipavaloritem
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid";

        $sql .= " where catid =" . $key[catid];

        if (!empty($dados['forid'])) {
            $sql .= "and ctt.forid =" . $dados['forid'];
        }

        if (!empty($dados['camid'])) {
            $sql .= " and pad.camid =" . $dados['camid'];
        }

         if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }

        $sql .= " and pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') order by fornome";
        $registros = $db->carregar($sql);

        $total = 0;
        if (!empty($registros)) {
            $html .=

                    '<tr>
                <td style="text-align: center;font-size: 16px;padding: 10px 10px 10px 0" colspan="6">' . $key[catdsc] . '</td>
            </tr>
            <tr >
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 150px">Agência</td>
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                <td style="font-size: 9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
            </tr>';

            foreach ($registros as $value) {
                $totalGeral += $value[ipavaloritem];
                $total += $value[ipavaloritem];
                $html .=
                        '<tr>
                    <td style="font-size: 9px;text-align: center">' . $value[fornecedor] . '</td>
                    <td style="font-size: 9px;text-align: center">' . $value[orgdsc] . '</td>
                    <td style="font-size: 9px;text-align: center">' . $value[padnumsidoc] . '</td>
                    <td style="font-size: 9px;text-align: center">' . $value[ipanumitempad] . '</td>
                    <td style="font-size: 9px;text-align: left">' . $value[ipadsc] . '</td>
                    <td style="font-size: 9px;text-align: right">' . number_format($value[ipavaloritem], 2, ',', '.') . '</td>
                </tr>';
            }
            $html .= '<tr><td colspan="6"style="font-size: 9px;background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($total, 2, ',', '.') . '</b></td> </tr>';
        }

    };
            $html .= '<tr><td colspan="6"style="font-size: 9px;background-color: #cccccc;text-align: center"><b>Total Geral: R$ ' . number_format($totalGeral, 2, ',', '.') . '</b></td> </tr>';
    $html .= '</table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html .= '</body>';
    }

    return $html;
}

function listaCampanhaDetalhada($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Campanha Detalhado</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1" >';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1">';
    }

    $sql = "select camtitulo, camid from publicidade.campanha";

    if (!empty($dados['camid'])) {
        $sql .= " where camid =" . $dados['camid'];
    }

    $sql .= " order by camtitulo";
    $campanhas = $db->carregar($sql);
    $total_geral = 0;
    foreach ($campanhas as $key) {
        $sqlPrincipal = "select distinct ite.ipanumitempad,
padnumsidoc,
(select forn.fornome from publicidade.fornecedor forn where ite.forid = forn.forid) as agencia,
orgdsc,
ite.ipadsc,
catdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor, 
fipnumfaturafornecedor, 
fipnumfaturaagencia,
case WHEN  pip.pipstatus = 'I' THEN '' ELSE pipnumordembancaria END as pipnumordembancaria
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid";

        $sqlPrincipal .= " where cam.camid =" . $key[camid];

        if (!empty($dados['camid'])) {
            $sqlPrincipal .= " and cam.camid =" . $dados['camid'];
        }

        if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
            $sqlPrincipal .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
        }

        if (!empty($dados['ano'])) {
            $sqlPrincipal .= " and pad.padano =" . $dados['ano'];
        }
        $sqlPrincipal .= " and pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') order by ipanumitempad asc";
        $registros = $db->carregar($sqlPrincipal);
        $total = 0;
        if (!empty($registros)) {
            $html .=
                    '<tr>
                <td style="text-align: center;font-size: 16px;padding: 10px 10px 10px 0" colspan="10">' . $key[camtitulo] . '</td>
            </tr>
            <tr >
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 80px">Categoria</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>
            </tr>';

            foreach ($registros as $value) {
                $total += $value[valor];
                $total_geral += $value[valor];
                $html .=
                        '<tr>
                    <td style="font-size:9px;text-align: center">' . $value[ipanumitempad] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[padnumsidoc] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[agencia] . '</td>       
                    <td style="font-size:9px;text-align: center">' . $value[orgdsc] . '</td>                    
                    <td style="font-size:9px;text-align: left">' . $value[ipadsc] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[catdsc] . '</td>
                    <td style="font-size:9px;text-align: right">' . number_format($value[valor], 2, ',', '.') . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturafornecedor] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[fipnumfaturaagencia] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[pipnumordembancaria] . '</td>
                </tr>';
            }

            $html .=
                    '<tr><td colspan="10" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($total, 2, ',', '.') . '</b></td> </tr>';
        }
    };
    $html .=
            '<tr><td colspan="10" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total Geral: R$ ' . number_format($total_geral, 2, ',', '.') . '</b></td> </tr>
    </table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table></body>';
    }

    return $html;
}

function listaCampanha($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Campanha Resumido</b><br><br><table class="tabela" style="width: 85% !important;" align="center" border="1">
    <thead>
    <th style="font-size:9px;text-align: center">Campanha</th><th style="font-size:9px;text-align: center">Órgão</th><th style="font-size:9px;text-align: right">Valor (R$)</th>
</thead>';
    } else {
        $html = '<table class="tabela" style="width: 85% !important;" align="center" border="1">
    <thead>
    <th style="font-size:9px;text-align: center">Campanha</th><th style="font-size:9px;text-align: center">Órgão</th><th style="font-size:9px;text-align: center">Valor</th>
</thead>';
    }

    $sql = "select sum(valor) as valorcampanha, camtitulo,
orgdsc from(
select distinct ite.ipanumitempad,
camtitulo,
orgdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
        where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P')";

    if (!empty($dados['forid'])) {

        $sql .= "and ctt.forid =" . $dados['forid'];
    }

    if (!empty($dados['ano'])) {
        $sql .= " and pad.padano =" . $dados['ano'];
    }

    if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
        $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
    }

    $sql .= " order by camtitulo
        ) a group by camtitulo,
orgdsc order by camtitulo";

    $registros = $db->carregar($sql);
    $total = 0;

    if (!empty($registros)) {
        $valor_total = 0;
        foreach ($registros as $value) {
            $valor_total += $value[valorcampanha];
            $html .=
                    '<tr>
            <td style="font-size:9px;text-align: center">' . $value[camtitulo] . '</td>
            <td style="font-size:9px;text-align: center">' . $value[orgdsc] . '</td>
            <td style="font-size:9px;text-align: right">' . number_format($value[valorcampanha], 2, ',', '.') . '</td>
        </tr>';
        }
    }
    $html .=
            '<tr><td colspan="3" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($valor_total, 2, ',', '.') . '</b></td> </tr>
</table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table></body>';
    }

    return $html;
}

function listaSituacao($dados, $tipoRelatorio) {
    global $db;

    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Situação</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1" >';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1" >';
    }

    $sql = "select distinct CASE WHEN ite.ipastatus = 'A' THEN 'Aprovado' 
			      
WHEN ite.ipastatus = 'C' THEN 'Cancelado'
				      
WHEN ite.ipastatus = 'F' THEN 'Faturado'
					      
 WHEN ite.ipastatus = 'P' THEN 'Pago'
						      
WHEN ite.ipastatus = 'G' THEN 'Glosado'							  
	      
END AS situacao_descricao,  ite.ipastatus as situacao_id from  publicidade.itenspad ite 
where ite.ipastatus in ('A','F','P') order by ipastatus";

    $situacoes = $db->carregar($sql);
    // ver($situacoes,d);
    $total_geral = 0;
    foreach ($situacoes as $key) {

        if ($key['situacao_id'] == 'A') {
            $sql = "select distinct ite.ipanumitempad,
padnumsidoc,
(select forn.fornome from publicidade.fornecedor forn where ite.forid = forn.forid) as agencia,
orgdsc,
ite.ipadsc,
catdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal  else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
               where pad.padano >= '2015' and ite.ipastatus ='" . $key['situacao_id'] . "' ";
        }
        if ($key['situacao_id'] == 'F') {
            $sql = "select distinct ite.ipanumitempad,
padnumsidoc,
(select forn.fornome from publicidade.fornecedor forn where ite.forid = forn.forid) as agencia,
orgdsc,
ite.ipadsc,
catdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor,
fip.fipnumfaturafornecedor, 
fip.fipnumfaturaagencia
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
               where pad.padano >= '2015' and ite.ipastatus IN ('" . $key['situacao_id'] . "','G')";
            
        }
        if ($key['situacao_id'] == 'P') {
            $sql = "select distinct ite.ipanumitempad,
padnumsidoc,
(select forn.fornome from publicidade.fornecedor forn where ite.forid = forn.forid) as agencia,
orgdsc,
ite.ipadsc,
catdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor,
fip.fipnumfaturafornecedor, 
fip.fipnumfaturaagencia,
case WHEN  pip.pipstatus = 'I' THEN '' ELSE pipnumordembancaria END as pipnumordembancaria
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
               where pad.padano >= '2015' and ite.ipastatus ='" . $key['situacao_id'] . "'";
        }

        if (!empty($dados['camid'])) {
            $sql .= " and cam.camid =" . $dados['camid'];
        }

        if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
            $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
        }

        if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }
        $sql .= " order by ipanumitempad asc";

       // ver($sql,d);
        $registros = $db->carregar($sql);
        $total = 0;

        if (!empty($registros)) {
            $html .=

                    '<tr>
                <td style="text-align: center;font-size: 16px;padding: 10px 10px 10px 0" colspan="10">Situação: <b>' . $key[situacao_descricao] . '</b>' . '</td>
            </tr>
            <tr >';

            if ($key['situacao_id'] == 'A') {
                $html .=
                    '<td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 80px">Categoria</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>';
            }

            if ($key['situacao_id'] == 'F') {
                 $html .=
                    '<td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 80px">Categoria</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>';
            }
            if ($key['situacao_id'] == 'P') {
                $html .=
                    '<td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Órgão</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 80px">Categoria</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor (R$)</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura do Fornecedor</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da Fatura da Agência</td>
                    <td style="font-size:9px;text-align: center;font-weight: bold;width: 50px">Nº da OB</td>';
            }
            $html .=
                    '</tr>';

            foreach ($registros as $value) {
                $total += $value[valor];
                $total_geral += $value[valor];
                $html .=
                        ' <tr>
                    <td style="font-size:9px;text-align: center">' . $value[ipanumitempad] . '</td>                    
                    <td style="font-size:9px;text-align: center">' . $value[padnumsidoc] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[agencia] . '</td>       
                    <td style="font-size:9px;text-align: center">' . $value[orgdsc] . '</td>                    
                    <td style="font-size:9px;text-align: left">' . $value[ipadsc] . '</td>
                    <td style="font-size:9px;text-align: center">' . $value[catdsc] . '</td>
                    <td style="font-size:9px;text-align: right">' . number_format($value[valor], 2, ',', '.') . '</td>';
                
                if($value[fipnumfaturafornecedor]){
                    $html.='<td style="font-size:9px;text-align: center">' . $value[fipnumfaturafornecedor] . '</td>';
                }else{
                    $html.='<td style="font-size:9px;text-align: center"><b>X</b></td>';

                }
                
                if($value[fipnumfaturaagencia]){
                    $html.='<td style="font-size:9px;text-align: center">' . $value[fipnumfaturaagencia] . '</td>';
                }else{
                    $html.='<td style="font-size:9px;text-align: center"><b>X</b></td>';

                }
                
                if($value[pipnumordembancaria]){
                    $html.='<td style="font-size:9px;text-align: center">' . $value[pipnumordembancaria] . '</td>';
                }else{
                    $html.='<td style="font-size:9px;text-align: center"><b>X</b></td>';

                }
               $html .= '</tr>';
            }
            $html .=
                    '<tr><td style="font-size:9px;background-color: #cccccc;text-align: center" colspan="10"><b>Total ' . $key[situacao_descricao] . ': R$ ' . number_format($total, 2, ',', '.') . '</b></td> </tr>';
        }
    };
    $html .=
            '<tr><td colspan="10" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total Geral: R$ ' . number_format($total_geral, 2, ',', '.') . '</b></td> </tr>
    </table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html .= '</body>';
    }

    return $html;
}

function listaServicos($dados, $tipoRelatorio) {
    global $db;

    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Relatório Serviços Glosados</b><br><br><table class="tabela" style="width:100% !important;" align="center" border= "1">';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1">';
    }

    $sql = "select distinct ite.ipanumitempad, 
padnumsidoc,
camtitulo,
ite.ipadsc, 
catdsc, 
gipvalor,
fip.fipvalortotal
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
where pad.padano >= '2015'";

    if (!empty($dados['forid'])) {

        $sql .= "and ctt.forid =" . $dados['forid'];
    }


    if (!empty($dados['ano'])) {
        $sql .= " and pad.padano =" . $dados['ano'];
    }

    if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
        $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
    }

    $sql .= " order by ipanumitempad asc;";

    $registros = $db->carregar($sql);

    $totalVT = 0;
    $totalVG = 0;
    $totalVF = 0;
    if (!empty($registros)) {
        $html .=
                '<tr >
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 110px">Nº Item</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 130px">Processo</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Campanha</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 1200px">Descrição</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 150px">Categoria</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor Glosado (R$)</td>
            <td style="font-size:9px;text-align: center;font-weight: bold;width: 100px">Valor Faturado (R$)</td>

        </tr>';

        foreach ($registros as $value) {
            $totalVT += $value[valor];
            $totalVG += $value[gipvalor];
            $totalVF += $value[fipvalortotal];
            // <td style="text-align: right">R$ ' . number_format($value[fipvalortotal], 2, ',', '.') . ' </td>
            $html .=
                    '<tr>
                <td style="font-size:9px;text-align: center">' . $value[ipanumitempad] . ' </td>
                <td style="font-size:9px;text-align: center">' . $value[padnumsidoc] . '  </td>
                <td style="font-size:9px;text-align: center">' . $value[camtitulo] . '  </td>
                <td style="font-size:9px;text-align: left">' . $value[ipadsc] . ' </td>
                <td style="font-size:9px;text-align: center">' . $value[catdsc] . ' </td>
                <td style="font-size:9px;text-align: right">' . number_format($value[gipvalor], 2, ',', '.') . ' </td>
                <td style="font-size:9px;text-align: right">' . number_format($value[fipvalortotal], 2, ',', '.') . ' </td>

            </tr>';
        }

        $html .=
                '<tr>
            <td colspan="5" style="text-align: right;background-color: #cccccc;"><b>Total (R$):</b></td>
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalVG, 2, ',', '.') . '</b></td> 
            <td style="background-color: #cccccc;text-align: right"><b>' . number_format($totalVF, 2, ',', '.') . '</b></td> 

        </tr>';
    }
    $html .= '</table>';

    if ($tipoRelatorio == 'html') {
        $html .= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html .= '</body>';
    }

    return $html;
}

function listaCategoria($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos por Categoria Sintético</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1" >
                       <thead>
    <th style="text-align: center">Categoria</th><th>Percentual</th><th>Valor</th>
</thead>
';
    } else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" border="1">
                       <thead>
    <th style="font-size:9px;text-align: center">Categoria</th><th style="font-size:9px;text-align: center">Percentual</th><th style="font-size:9px;text-align: center">Valor (R$)</th>
</thead>';
    }

    $sql = "select sum(valor) as parcial,
trunc((sum(valor)/(select sum(case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal - coalesce(gipvalor,0) else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end) 
from publicidade.itenspad ite 
join  publicidade.categoria cat on cat.catid = ite.catid 
join publicidade.pad pad on pad.padid =ite.padid   
               left join publicidade.faturamentoitempad fip on fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
               left join publicidade.glosaitempad glo on glo.ipaid = ite.ipaid
               left join publicidade.pagamentoitempad pip on pip.ipaid = ite.ipaid AND pip.pipstatus = 'A' 
where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P')";

    if (!empty($dados['forid'])) {
        $sql .= " and forid =" . $dados['forid'];
    }

    if (!empty($dados['camid'])) {
        $sql .= " and camid =" . $dados['camid'];
    }
    if (!empty($dados['categoria'])) {
        $sql .= " and catid =" . $dados['categoria'];
    }
    if (!empty($dados['ano'])) {
        $sql .= " and pad.padano =" . $dados['ano'];
    }

    $sql .= "))*100,2) as percentual,catdsc from(
select 
catdsc,
case WHEN trim(pip.pipvalorfaturado)::numeric > 0 then trim(pip.pipvalorfaturado)::numeric 
when coalesce(fip.fipvalortotal,0) > 0 then fip.fipvalortotal else (ite.ipavalorservico + ite.ipavalorhonorario) - coalesce(gipvalor,0) end as valor
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
        where pad.padano >= '2015' and catstatus = 'A' 
        and ipastatus in ('A' , 'F', 'G', 'P')";
    
    if (!empty($dados['forid'])) {
        $sql .= " and ctt.forid =" . $dados['forid'];
    }
    if (!empty($dados['camid'])) {
        $sql .= " and pad.camid =" . $dados['camid'];
    }
    if (!empty($dados['categoria'])) {
        $sql .= " and cat.catid =" . $dados['categoria'];
    }

    if (!empty($dados['ano'])) {
        $sql .= " and pad.padano =" . $dados['ano'];
    }

    $sql .= " order by camtitulo
        ) a group by catdsc
order by catdsc";

   //ver($sql,d);
    $registros = $db->carregar($sql);

    $total = 0;
    if (!empty($registros)) {
        foreach ($registros as $value) {

            $total += $value[parcial];

            $html .=
                    '<tr>
                <td style="font-size:9px;text-align: center">' . $value[catdsc] . '</td>
                <td style="font-size:9px;text-align: center">' . $value[percentual] . '%</td>
                <td style="font-size:9px;text-align: right">' . number_format($value[parcial], 2, ',', '.') . '</td>
            </tr>';
        }
    }
    $html .=
            '<tr><td colspan="3" style="font-size:9px;background-color: #cccccc;text-align: center"><b>Total:' . number_format($total, 2, ',', '.') . '</b></td> </tr>
    </table>';

    if ($tipoRelatorio == 'html') {
        $html.= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html.= '</body>';
    }

    return $html;
}

function listaPUP($dados, $tipoRelatorio) {
    global $db;
    if ($tipoRelatorio == 'html') {
        $html = '<html>
            <head>
            <script type="text/javascript">

(function()
{
  if( window.localStorage )
  {
    if( !localStorage.getItem( "firstLoad" ) )
    {
      localStorage[ "firstLoad" ] = true;
      window.location.reload();
    }  
    else
      localStorage.removeItem( "firstLoad" );
  }
})();

</script>
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> '. NOME_SISTEMA. ' </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Gastos PUP Sintético</b><br><br><table class="tabela" style="width:100% !important;" align="center" border="1" >
                       <thead>
    <th>Tipo de Serviço</th><th>Valor</th><th>Tipo de Serviço</th><th>Valor</th>
</thead>
';
    }else {
        $html = '<table class="tabela" style="width:100% !important;" align="center" >
                       <thead>
    <th>Tipo de Serviço</th><th>Valor</th><th>Tipo de Serviço</th><th>Valor</th>
</thead>';
    }

     $total_geral = 0; 



$sql = "select sum(valor_servico) as total_servico,
sum(valor_honorario) as total_honorario , sum(valor) as valor, tsedsc from(
select 
tsedsc,
case WHEN fip.fipvalortotal > 0 then fip.fipvalortotal else ipavalortotal end as valor,
case WHEN gipvalor > 0 then ipavalorservico - gipvalor else ipavalorservico end as valor_servico,
ite.ipavalorhonorario as valor_honorario,
ite.ipavalortotal as valor_item,
gipvalor as valor_glosado
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
left join publicidade.tiposervico tse on tse.tseid = ite.tseid 
where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') and tsedsc  = 'PRODUÇÃO' ";

if (!empty($dados['forid'])) {
    $sql .= " and ctt.forid =" . $dados['forid'];
}

if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
    $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
}

if (!empty($dados['ano'])) {
    $sql .= " and pad.padano =" . $dados['ano'];
}
$sql .= "  order by tsedsc
        ) a group by tsedsc
order by tsedsc
";

$producao = $db->carregar($sql);
$totalProducao = $producao[0][total_servico];
$total_geral = $producao[0][total_servico];

       $html .= '<tr><td style="text-align: left">'; 
       if ($producao[0][tsedsc] == 'PRODUÇÃO') {
            $html .= $producao[0][tsedsc];
    } else {  $html .= 'PRODUÇÃO'; 
    
       }
         $html .= '</td> 
        <td style="text-align: left">';
         if ($producao[0][tsedsc] == 'PRODUÇÃO') {
             $html .= 'R$ ' . number_format($producao[0][total_servico], 2, ',', '.');
        } else {
             $html .= 'R$ 0,00';
        } 
         $html .= "</td>";

        $sql = "select sum(valor_servico) as total_servico,
sum(valor_honorario) as total_honorario , sum(valor) as valor, tsedsc from(
select 
tsedsc,
case WHEN fip.fipvalortotal > 0 then fip.fipvalortotal else ite.ipavalorservico + ite.ipavalorhonorario  end as valor,
case WHEN gipvalor > 0 then ipavalorservico - gipvalor else ipavalorservico end as valor_servico,
ite.ipavalorhonorario as valor_honorario,
ite.ipavalortotal as valor_item,
gipvalor as valor_glosado
FROM publicidade.itenspad ite
JOIN publicidade.pad PAD ON PAD.padid =ite.padid
JOIN publicidade.campanha cam ON cam.camid =PAD.camid
JOIN publicidade.contrato ctt ON ctt.cttid = PAD.cttid
JOIN publicidade.fornecedor forn ON forn.forid = ite.forid
JOIN publicidade.categoria cat ON cat.catid = ite.catid
LEFT JOIN publicidade.faturamentoitempad fip ON fip.ipaid = ite.ipaid AND fip.fipstatus = 'A'
LEFT JOIN publicidade.pagamentoitempad pip ON pip.ipaid = ite.ipaid AND pip.pipstatus = 'A'
LEFT JOIN publicidade.vworgao org ON org.orgid::integer = cam.orgid::integer
LEFT JOIN publicidade.glosaitempad glo ON glo.ipaid = ite.ipaid
left join publicidade.tiposervico tse on tse.tseid = ite.tseid 
where pad.padano >= '2015' and ipastatus in ('A' , 'F', 'G', 'P') and tsedsc  = 'MÍDIA'";
        
        if (!empty($dados['forid'])) {
            $sql .= " and ctt.forid =" . $dados['forid'];
        }

        if (!empty($dados['datainicial']) && !empty($dados['datafinal'])) {
            $sql .= " and pademissao between '{$dados['datainicial']}' and '{$dados['datafinal']}'";
        }

        if (!empty($dados['ano'])) {
            $sql .= " and pad.padano =" . $dados['ano'];
        }
        $sql .= "  order by tsedsc
        ) a group by tsedsc
order by tsedsc
";
        $midia = $db->carregar($sql);
        
        $totalMidia = $midia[0][total_servico];
        $total_geral = $total_geral + $midia[0][total_servico];
             
         $html .= '<td style="text-align: left">';
          if ($midia[0][tsedsc] == 'MÍDIA') {
             $html .= $midia[0][tsedsc];
        } else  $html .= 'MÍDIA';
         $html .= '</td>                    
        <td style="text-align: left">';
        if ($midia[0][tsedsc] == 'MÍDIA') {
             $html .= 'R$ ' . number_format($midia[0][total_servico], 2, ',', '.');
        } else {
             $html .= 'R$ 0,00';
        }  $html .= "</td>
    </tr>
    <tr>";
          
        $totalProducao += $producao[0][total_honorario];
        $total_geral = $total_geral + $producao[0][total_honorario];

        
        $html .= '<td style="text-align: left">HONORÁRIOS PRODUÇÃO</td>';                   
        $html .= '<td style="text-align: left">';
        if ($producao[0][tsedsc] == 'PRODUÇÃO') {
            $html .= 'R$ ' . number_format($producao[0][total_honorario], 2, ',', '.');
        } else {
             $html .= 'R$ 0,00';
        } 
         $html .= '</td>';

        $totalMidia = $totalMidia + $midia[0][total_honorario];
        $total_geral = $total_geral + $midia[0][total_honorario];

         $html .= '<td style="text-align: left">HONORÁRIOS MÍDIA</td>                    
        <td style="text-align: left">';
         if ($midia[0][tsedsc] == 'MÍDIA') {
             $html .= 'R$ ' . number_format($midia[0][total_honorario], 2, ',', '.');
        } else {
             $html .= 'R$ 0,00';
        }  $html .= '</td>
    </tr>
    <tr><td colspan="2" style="background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($totalProducao, 2, ',', '.').'</b></td><td colspan="2" style="background-color: #cccccc;text-align: center"><b>Total: R$ ' . number_format($totalMidia, 2, ',', '.').'</b></td> </tr>';

    $html .= '<tr><td colspan="4" style="background-color: #cccccc;text-align: center"><b>Total Geral: R$ ';
    $html .= number_format($total_geral, 2, ',', '.');
    $html .= '</b></td> </tr></table>';

       if ($tipoRelatorio == 'html') {
        $html.= '<br><br><div class=notprint><input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();"></div></td></tr></table>';
        $html.= '</body>';
    }
    
    return $html;
}

function montaComboExRel (){
    $html = '<select id="ano" style="width: auto" class="CampoEstilo chosen-container" name="ano">
                       <option value="">Selecione...</option>
                       <option value="2015">2015</option>
                   </select>';
    return $html;
}
?>