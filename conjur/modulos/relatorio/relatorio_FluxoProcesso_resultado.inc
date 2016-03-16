<?php 

/**
 * Monta request e gera os resultados para a query 
 * da função "monta_sql_fluxo()" a seguir
 *
 * @param string $padrao
 * @param array $params
 * @author Sávio Resende
 */
function monta_request_sql_fluxo( $padrao, $params, $lista = 'advid' ){
  global $db;

  // advid || coonid
  if( $params['advid'] && $params['coonid'] ){
    $ent = array( 'advid', $params['advid'], 'coonid', $params['coonid'] );
  }else if( $params['advid'] ){
    $ent = array('advid',$params['advid']);
  }else if( $params['coonid'] ){
    $ent = array('coonid', $params['coonid']);
  }

  // distinção dos padrões
  switch( $padrao ){


    case "entrada": // ----------------------
      $_REQUEST = array(
          'htddata_inicio' => $params['htddatainicial'],
          'htddata_fim' => $params['htddatfinal'],
          'aedid_campo_flag' => 1,
          'aedid' => array(
                  WF_AEDID_ENCAMINHAR_ANALISE_GAB,
                  WF_AEDID_ENCAMINHAR_CGAA,
                  WF_AEDID_ENCAMINHAR_CGAC,
                  WF_AEDID_ENCAMINHAR_CGAE
              ),
          'agrupador' => array(
                  'aeddscrealizar',
                  'htddata',
                  'prcdesc'
              ),
          $ent[0] => $ent[1],
      );
      if( $ent[2] ){ $_REQUEST[ $ent[2] ] = $ent[3]; }

      if( $lista == 'coonid' ) $entrada = $db->carregar( monta_sql_fluxo_coordernacao() );
      else $entrada = $db->carregar( monta_sql_fluxo() );

      if( !$entrada ){
        $retorno = array(
          'total' => '0',
          'htddata' => '-'
        );
      }else{
        $retorno = array(
          'total' => count($entrada),
          'htddata' => $entrada[0]['htddata']
        );
      }
      break;


    case "saida": // ----------------------
      $_REQUEST = array(
          'htddata_inicio' => $params['htddatainicial'],
          'htddata_fim' => $params['htddatfinal'],
          'aedid_campo_flag' => 1,
          'aedid' => array(
                  WF_AEDID_ENCERRAR_PROCESSO
              ),
          'agrupador' => array(
                  'aeddscrealizar',
                  'htddata',
                  'prcdesc'
              ),
          $ent[0] => $ent[1],
      );
      if( $ent[2] ){ $_REQUEST[ $ent[2] ] = $ent[3]; }

      if( $lista == 'coonid' ) $saida = $db->carregar( monta_sql_fluxo_coordernacao() );
      else $saida = $db->carregar( monta_sql_fluxo() );

      if( !$saida ){
        $retorno = array(
          'total' => '0',
          'htddata' => '-'
        );
      }else{
        $retorno = array(
          'total' => count($saida),
          'htddata' => $saida[0]['htddata']
        );
      }
      break;


    case "arquivado": // ----------------------
      $_REQUEST = array(
            'htddata_inicio' => $params['htddatainicial'],
            'htddata_fim' => $params['htddatfinal'],
            'aedid_campo_flag' => 1,
            'coonid' => $params['coonid'],
            'advid' => $params['advid'],
          $ent[0] => $ent[1],
        );
      if( $ent[2] ){ $_REQUEST[ $ent[2] ] = $ent[3]; }

      $where = "";
      if( $lista == 'coonid' ){
        
        $_REQUEST['advid'] = (!is_array($_REQUEST['advid']) && $_REQUEST['advid'] )?array($_REQUEST['advid']):$_REQUEST['advid'];
        
        $where_data = ' 1=1 ';
        if( $_REQUEST['htddata_inicio'] )
          $where_data .= " AND htddata >= TIMESTAMP '{$_REQUEST['htddata_inicio']} 00:00:00' ";
        if( $_REQUEST['htddata_fim'] )
          $where_data .= " AND htddata <= TIMESTAMP '{$_REQUEST['htddata_fim']} 23:59:59' ";
        if( $_REQUEST['coonid'] )
          $where_data .= " AND cooid = '{$_REQUEST['coonid']}' ";
        if( $_REQUEST['advid'] )
          $where_data .= " AND advid IN (".implode(',',$_REQUEST['advid']).") ";

        $sql = "
          SELECT * FROM (
            SELECT 
              prc.prcid,
              ( SELECT MAX(subhad.advid) 
              FROM conjur.historicoadvogados subhad
              WHERE subhad.prcid = prc.prcid ) AS advid,
              prc.cooid,
              (  SELECT MAX(subhis2.htddata)
              FROM workflow.historicodocumento subhis2
              WHERE subhis2.docid = doc.docid ) AS htddata
            FROM conjur.processoconjur prc
            LEFT JOIN conjur.estruturaprocesso est ON est.prcid = prc.prcid
            LEFT JOIN workflow.documento doc ON doc.docid = est.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE 
              prc.prcstatus = 'A' 
              AND doc.esdid = ".WF_ESDID_ARQUIVADO."
          ) qry WHERE {$where_data} " ;

      }else{ 

         $_REQUEST['advid'] = (!is_array($_REQUEST['advid']) && $_REQUEST['advid'] )?array($_REQUEST['advid']):$_REQUEST['advid'];
        
        $where_data = ' 1=1 ';
        if( $_REQUEST['htddata_inicio'] )
          $where_data .= " AND htddata >= TIMESTAMP '{$_REQUEST['htddata_inicio']} 00:00:00' ";
        if( $_REQUEST['htddata_fim'] )
          $where_data .= " AND htddata <= TIMESTAMP '{$_REQUEST['htddata_fim']} 23:59:59' ";
        if( $_REQUEST['coonid'] )
          $where_data .= " AND cooid = '{$_REQUEST['coonid']}' ";
        if( $_REQUEST['advid'] )
          $where_data .= " AND advid IN (".implode(',',$_REQUEST['advid']).") ";

        $sql = "
          SELECT * FROM (
            SELECT 
              prc.prcid,
              ( SELECT MAX(subhad.advid) 
              FROM conjur.historicoadvogados subhad
              WHERE subhad.prcid = prc.prcid ) AS advid,
              prc.cooid,
              (  SELECT MAX(subhis2.htddata)
              FROM workflow.historicodocumento subhis2
              WHERE subhis2.docid = doc.docid ) AS htddata
            FROM conjur.processoconjur prc
            LEFT JOIN conjur.estruturaprocesso est ON est.prcid = prc.prcid
            LEFT JOIN workflow.documento doc ON doc.docid = est.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            WHERE 
              prc.prcstatus = 'A' 
              AND doc.esdid = ".WF_ESDID_ARQUIVADO."
          ) qry WHERE {$where_data} " ;

      }
      // ver($sql);
      $padrao = $db->carregar( $sql );

      if( !$padrao ){
        $retorno = array(
          'total' => '0',
          'htddata' => '-'
        );
      }else{
        $retorno = array(
          'total' => count($padrao),
          'htddata' => '-' //$padrao[0]['htddata']
        );
      }
    break;

  }

  return $retorno;

}
 
 /**
  * Busca nome do advogado para o caso de 
  * resultados zerados na query gerada em "monta_sql_fluxo()"
  *
  * @param integer $advid
  * @author Sávio Resende
  */
function buscaNomeAdvogado( $advid ){
  global $db;

  $sql = " SELECT ent.entnome 
    FROM conjur.advogados adv 
    INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
    WHERE adv.advid = ".$advid." ";

  return $db->pegaUm( $sql );
}

/**
 * Busca nome do advogado para o caso de 
 * resultados zerados na query gerada em "monta_sql_fluxo()"
 *
 * @param integer $advid
 * @author Sávio Resende
 */
function buscaNomeCoordenacao( $coonid ){
  global $db;

  $sql = " SELECT coo.coodsc 
    FROM conjur.coordenacao coo
    WHERE coo.coonid = ".$coonid." ";

  return $db->pegaUm( $sql );
}

/**
 * Monta SQL baseada no histórico - para advogados e mec
 *
 * .copia da funcao que consta no arquivo relatorio_historicoProcessos_resultado.inc adaptada para trazer solicitações
 * .parâmetros baseados no request
 * .obs.: qualquer modificação feita na função 
 *  "monta_sql()" do arquivo relatorio_historicoProcessos_resultado.inc 
 *  deve ser analisara e aplicada aqui também
 *
 * @author Sávio Resende
 */
function monta_sql_fluxo(){

  extract($_REQUEST);
  // ver($_REQUEST,d);
  $where = array();
  
  // coordenação
  if( $coonid[0] && $coonid_campo_flag ){
    array_push($where, " co1.coonid " . (!$coonid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coonid ) . "') ");
  }
  
  // advogado
  if( $advid ){
    array_push($where, " adv.advid = '" . $advid . "' ");
  }

  // coordenacao
  if( $coonid ){
    array_push($where, " co2.coonid = '" . $coonid . "' ");
  }
  
  // Ação
  if( $aedid[0] && $aedid_campo_flag ){
    array_push($where, " aca.aedid " . (!$aedid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $aedid ) . "') ");
  }
  
  // período
  if( $htddata_inicio && $htddata_fim ){
    $htddata_inicio = formata_data_sql($htddata_inicio);
    $htddata_fim = formata_data_sql($htddata_fim);
    array_push($where, " his.htddata >= TIMESTAMP '{$htddata_inicio}' AND his.htddata <= TIMESTAMP '{$htddata_fim} 23:59:59' ");
  }
  
  if($_POST['req'] == 'geraxls'){
    $prcnumsidoc = "prc.prcnumsidoc";
    $data = "to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
  }else{
    $prcnumsidoc = "'<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '<span style=\"display:none\" >' || his.hstid || '</span></a>' as prcnumsidoc";
    $data = "'<span style=\"display:none\" >' || his.htddata || '</span>' || to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
  }
  
  // monta o sql 
  $sql = "SELECT
          prc.prcid,
          TO_CHAR(his.htddata,'MM/YYYY') AS htddata
       FROM
               workflow.historicodocumento his
           LEFT  JOIN workflow.comentariodocumento  com ON com.hstid = his.hstid
           INNER JOIN workflow.documento      doc ON doc.docid = his.docid
           INNER JOIN conjur.estruturaprocesso    est ON est.docid = his.docid
           INNER JOIN conjur.processoconjur     prc ON prc.prcid = est.prcid
           LEFT  JOIN workflow.acaoestadodoc    aca ON aca.aedid = his.aedid
           LEFT  JOIN workflow.estadodocumento    eca ON eca.esdid = aca.esdidorigem
           LEFT  JOIN ( SELECT max(hstid) as hstid, docid 
                  FROM workflow.historicodocumento 
                  WHERE aedid = ".ACAO_ENCAMINHAR_PARA_ADVOGADO."  
                  GROUP BY docid)       htsadv ON htsadv.docid = doc.docid 
           LEFT  JOIN conjur.historicoadvogados   had ON had.hstid = htsadv.hstid
           LEFT  JOIN conjur.advogados        adv ON adv.advid = had.advid
           LEFT  JOIN conjur.coordenacao      co2 ON co2.coonid = adv.coonid
           LEFT  JOIN entidade.entidade       ent ON ent.entid = adv.entid
           LEFT  JOIN conjur.historicocoordenacoes  hco ON hco.hstid = his.hstid
           INNER JOIN conjur.coordenacao      co1 ON co1.coonid = hco.coonid
           WHERE
        doc.tpdid = " . NOVO_WORKFLOW . "
        AND his.htddata > TIMESTAMP '2012-01-01 00:00:00'
        ".($where[0] ? ' AND' . implode(' AND ', $where) : '' )."
      GROUP BY prc.prcid, his.htddata, ent.entnome ";
  // dbg($sql,1);
  return $sql;
}

/**
 * Monta SQL baseada no histórico - para coordenação
 *
 * .copia da funcao que consta no arquivo relatorio_historicoProcessos_resultado.inc adaptada para trazer solicitações
 * .parâmetros baseados no request
 * .obs.: qualquer modificação feita na função 
 *  "monta_sql()" do arquivo relatorio_historicoProcessos_resultado.inc 
 *  deve ser analisara e aplicada aqui também
 *
 * @author Sávio Resende
 */
function monta_sql_fluxo_coordernacao(){

  extract($_REQUEST);
  // ver($_REQUEST,d);
  $where = array();
  
  // coordenação
  if( $coonid[0] && $coonid_campo_flag ){
    array_push($where, " co1.coonid " . (!$coonid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coonid ) . "') ");
  }
  
  // advogado
  if( $advid ){
    array_push($where, " adv.advid = '" . $advid . "' ");
  }

  // coordenacao
  if( $coonid ){
    array_push($where, " co1.coonid = '" . $coonid . "' ");
  }
  
  // Ação
  if( $aedid[0] && $aedid_campo_flag ){
    array_push($where, " aca.aedid " . (!$aedid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $aedid ) . "') ");
  }
  
  // período
  if( $htddata_inicio && $htddata_fim ){
    $htddata_inicio = formata_data_sql($htddata_inicio);
    $htddata_fim = formata_data_sql($htddata_fim);
    array_push($where, " his.htddata >= TIMESTAMP '{$htddata_inicio}' AND his.htddata <= TIMESTAMP '{$htddata_fim} 23:59:59' ");
  }
  
  if($_POST['req'] == 'geraxls'){
    $prcnumsidoc = "prc.prcnumsidoc";
    $data = "to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
  }else{
    $prcnumsidoc = "'<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '<span style=\"display:none\" >' || his.hstid || '</span></a>' as prcnumsidoc";
    $data = "'<span style=\"display:none\" >' || his.htddata || '</span>' || to_char(his.htddata,'DD/MM/YYYY HH24:MI:SS') as data";
  }
  
  // monta o sql 
  $sql = "SELECT
          prc.prcid,
          TO_CHAR(his.htddata,'MM/YYYY') AS htddata
       FROM
               workflow.historicodocumento his
           LEFT  JOIN workflow.comentariodocumento  com ON com.hstid = his.hstid
           LEFT JOIN workflow.documento      doc ON doc.docid = his.docid
           LEFT JOIN conjur.estruturaprocesso    est ON est.docid = his.docid
           LEFT JOIN conjur.processoconjur     prc ON prc.prcid = est.prcid
           LEFT  JOIN workflow.acaoestadodoc    aca ON aca.aedid = his.aedid
           LEFT  JOIN workflow.estadodocumento    eca ON eca.esdid = aca.esdidorigem
           LEFT  JOIN ( SELECT max(hstid) as hstid, docid 
                  FROM workflow.historicodocumento 
                  WHERE aedid = ".ACAO_ENCAMINHAR_PARA_ADVOGADO."  
                  GROUP BY docid)       htsadv ON htsadv.docid = doc.docid 
           LEFT  JOIN conjur.historicoadvogados   had ON had.hstid = htsadv.hstid
           LEFT  JOIN conjur.advogados        adv ON adv.advid = had.advid
           LEFT  JOIN conjur.coordenacao      co2 ON co2.coonid = adv.coonid
           LEFT  JOIN entidade.entidade       ent ON ent.entid = adv.entid
           LEFT  JOIN conjur.historicocoordenacoes  hco ON hco.hstid = his.hstid
           LEFT JOIN conjur.coordenacao      co1 ON co1.coonid = hco.coonid
           WHERE
        doc.tpdid = " . NOVO_WORKFLOW . "
        AND his.htddata > TIMESTAMP '2012-01-01 00:00:00'
        ".($where[0] ? ' AND' . implode(' AND ', $where) : '' )."
      GROUP BY prc.prcid, his.htddata, ent.entnome ";

  // dbg($sql,1);
  return $sql;
}



function relatorioFluxoProcesso($post){
    global $db;
    
    extract($post);

    $params = array(
      'htddatainicial' => $htddatainicial,
      'htddatfinal' => $htddatfinal
    );

    $array = array();
    foreach ($advid as $key => $value) {
      
      $params['advid'] = $value;

      // entrada
      $padrao = 'entrada';
      $entradas = monta_request_sql_fluxo( $padrao, $params );
      // ver($entradas,d);

      // saida
      $padrao = 'saida';
      $saidas = monta_request_sql_fluxo( $padrao, $params );

      // arquivado
      $padrao = 'arquivado';
      $padroes = monta_request_sql_fluxo( $padrao, $params );

      array_push( $array, array(
          'entnome' => buscaNomeAdvogado( $params['advid'] ),
          // 'htddata' => $entradas['htddata'],
          'totalentrada' => $entradas['total'],
          'totalsaida' => $saidas['total'],
          'totalarquivado' => $padroes['total']
        ) );

    }
    // ver($array,d);

  	$param['totalLinhas'] = false;
    $param['ordena'] = false;
  	$alinhamento = array('','right','right','right');
    $cabecalho = array('Advogado', 'Entrada', 'Saída', 'Arquivado');
    $db->monta_lista($array, $cabecalho, '250','10', 'S', 'center', 'N', '', '', $alinhamento, '', $param);
} 

function relatorioFluxoProcessoCoordenacao($post){ 
    global $db;
    
    extract($post);

    $params = array(
      'htddatainicial' => $htddatainicial,
      'htddatfinal' => $htddatfinal
    );

    $array = array();
    $htddata = 0;
    $totalentrada = 0;
    $totalsaida = 0;
    $totalarquivado = 0;

    // ver($coonid,d);
    if( $coonid )
    foreach ($coonid as $key2 => $value2) {
      $advid = $db->carregar( "select advid from conjur.advogados where coonid = ".$value2." AND advstatus = 'A' " );
      $entnome = buscaNomeCoordenacao( $value2 );

      // if( $advid )
      // foreach ($advid as $key => $value) {
        
        // $params['advid'] = $value['advid'];
        $params['coonid'] = $value2;

        // entrada
        $padrao = 'entrada';
        $entradas = monta_request_sql_fluxo( $padrao, $params, 'coonid' );
        // ver($entradas,d);

        // saida
        $padrao = 'saida';
        $saidas = monta_request_sql_fluxo( $padrao, $params, 'coonid' );

        // arquivado
        $padrao = 'arquivado';
        $padroes = monta_request_sql_fluxo( $padrao, $params, 'coonid' );


        $htddata = ( !$htddata )? $entradas['htddata'] : $htddata;
        $totalentrada = $totalentrada + $entradas['total'];
        $totalsaida = $totalsaida + $saidas['total'];

      // } 

      array_push( $array, array(
            'entnome' => $entnome,
            // 'htddata' => $htddata,
            'totalentrada' => $totalentrada,
            'totalsaida' => $totalsaida,
            'totalarquivado' => $padroes['total']
          ) );

      $entnome = 0;
      $htddata = '-';
      $totalentrada = 0;
      $totalsaida = 0;
      $totalarquivad = 0;
    }

    // ver($array,d);

    $param['totalLinhas'] = false;
    $param['ordena'] = false;
    $alinhamento = array('','right','right','right');
    $cabecalho = array('Coordenação', 'Entrada', 'Saída', 'Arquivado');
    $db->monta_lista($array, $cabecalho, '250','10', 'S', 'center', 'N', '', '', $alinhamento, '', $param);
}

function relatorioFluxoProcessoMEC($post){ 
    global $db;
    
    extract($post);

    $params = array(
      'htddatainicial' => $htddatainicial,
      'htddatfinal' => $htddatfinal
    );

    $coonid = $db->carregar( "select coonid from conjur.coordenacao" );

    $array = array();
    $htddata = 0;
    $totalentrada = 0;
    $totalsaida = 0;
    $totalarquivado = 0;

    if( $coonid )
    foreach ($coonid as $key2 => $value2) {

      $advid = $db->carregar( "select advid from conjur.advogados where coonid = ".$value2['coonid']." AND advstatus = 'A' " );
      
      if( $advid )
      foreach ($advid as $key => $value) {
        
        $params['advid'] = $value['advid'];
        $params['coonid'] = $value2['coonid'];
        
        // entrada
        $padrao = 'entrada';
        $entradas = monta_request_sql_fluxo( $padrao, $params );
        // ver($entradas,d);

        // saida
        $padrao = 'saida';
        $saidas = monta_request_sql_fluxo( $padrao, $params );

        // arquivado
        $padrao = 'arquivado';
        $padroes = monta_request_sql_fluxo( $padrao, $params );

        $htddata = ( !$htddata )? $entradas['htddata'] : $htddata;
        $totalentrada = $totalentrada + $entradas['total'];
        $totalsaida = $totalsaida + $saidas['total'];
        $totalarquivado = $totalarquivado + $padroes['total'];

      }

    }

    array_push( $array, array(
          // 'htddata' => $htddata,
          'totalentrada' => $totalentrada,
          'totalsaida' => $totalsaida,
          'totalarquivado' => $totalarquivado
        ) );
    // ver($array,d);

    $param['totalLinhas'] = false;
    $param['ordena'] = false;
    $alinhamento = array('right','right','right');
    $cabecalho = array('Entrada', 'Saída', 'Arquivado');
    $db->monta_lista($array, $cabecalho, '250','10', 'S', 'center', 'N', '', '', $alinhamento, '', $param);
} 
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>



<?php if($_POST['ajax']): 

    if( $_POST['advid'] ){
      $_POST['advid'] = explode(';',$_POST['advid']);
    }

    if( $_POST['coonid'] ){
      $_POST['coonid'] = explode(';',$_POST['coonid']);
    }


    monta_titulo( 'Relatório de Fluxo Processos',''); ?>

    <div style="text-align:center;width:100%;"><h4>Período:&nbsp;&nbsp;<?=$_POST['htddatainicial'] . ' - ' .$_POST['htddatfinal']?></h4></div>

    <?php if($_POST['relA'] || $_POST['relT']){ ?>
    <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
    	<tr>
    		<td class="subtituloCentro" width="10%">Advogado</td>
    	</tr>
    </table>
    <div id="divFluxoProcesso">
    	<?php relatorioFluxoProcesso($_POST); ?>
    </div>
    <br>
    <?php } ?>

    <?php if($_POST['relC'] || $_POST['relT']){ ?>
    <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
    	<tr>
    		<td class="subtituloCentro" width="10%">Coordenação</td>
    	</tr>
    </table>
    <div id="divFluxoProcessoCoordenacao">
    	<?php relatorioFluxoProcessoCoordenacao($_POST); ?>
    </div>
    <br>
    <?php } ?>

    <?php if($_POST['relM'] || $_POST['relT']){ ?>
    <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
    	<tr>
    		<td class="subtituloCentro" width="10%">MEC</td>
    	</tr>
    </table>
    <div id="divFluxoProcessoMEC">
    	<?php relatorioFluxoProcessoMEC($_POST); ?>
    </div>
    <?php } 

    exit;?>

<?php else: 

    $_POST['ajax'] = 1;

    if( $_POST['advid'] ){
      $_POST['advid'] = implode(';',$_POST['advid']);
    }

    if( $_POST['coonid'] ){
      $_POST['coonid'] = implode(';',$_POST['coonid']);
    }
    ?>

    <div id="load" style="margin-top:50px;width:100%;text-align:center">
      <img src="../imagens/carregando.gif"/>&nbsp;&nbsp;Carregando...
    </div>
    <div id="conteudoCarregado"></div>
    <script>
      var post = <?=simec_json_encode($_POST)?>;
      jQuery.ajax({
        url:window.location.href,
        type: 'POST',
        data: post,
        success: function( resultado ){
          jQuery('#load').remove();
          jQuery('#conteudoCarregado').html( resultado );
        }
      });
    </script>

<?php endif; ?>