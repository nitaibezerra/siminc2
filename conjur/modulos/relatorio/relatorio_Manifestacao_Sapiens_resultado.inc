 <?php

include APPRAIZ . "conjur/classes/modelo/RelatorioConjurSapiens.class.inc";
// TODO: aplicar orientação feita no relatorio de manifestacao dos advogados para os outros (coordenacao e mec)

function relatorioManifestacao( $post ){

    // TODO: resolver esse impasse =/
    if( $post['relC'] ) unset($post['relC']);
    if( $post['relM'] ) unset($post['relM']);

    $ProcessoConjur = new ProcessoConjur();
    $ProcessoConjur->constroiRelatorio( 'RelatorioConjurManifestacoes', $post );
} 

function relatorioManifestacaoCoordenacao($post){
    global $db;
    
    extract($post);
    
    $aryWhere[] = " num.spdstatus = 'A' ";

    $arrayTPDID = array(
          TP_PARECER => 'parecer',
          TP_COTA => 'cota',
          TP_NOTA => 'nota',
          TP_DESPACHO => 'despacho',
          TP_OFICIO => 'oficio',
          TP_INFORMACAO => 'informacao' );
        
    if($coonid){
      $coonid = implode(',',$coonid);
   //   $aryWhere[] = "co2.coonid IN ({$coonid})";
    }

    $aryWhere[] = " adv.advid in ( SELECT a.advid
          FROM conjur.advogados a
          INNER JOIN entidade.entidade e ON e.entid=a.entid
          WHERE entstatus='A' and a.advstatus = 'A' ) ";

    if($htddatainicial && $htddatfinal){
      $aryWhere[] = "num.spdddatainclusao >= TIMESTAMP '{$htddatainicial}' AND num.spdddatainclusao <= TIMESTAMP '{$htddatfinal} 23:59:59'";
    }

    $sql = array();
    foreach ($arrayTPDID as $key => $value) {
        $where = ( is_array($aryWhere) && count($aryWhere) ? ' and ' . implode(' AND ', $aryWhere) : '' );
        $where .= " and td.tpdid in ($key) ";

        array_push($sql, " SELECT distinct on (p.prcid)
              p.prcid,
              coalesce(a.spaid,0) as nudidanexo,
              a.arqid, 
              td.tpdid,
              td.tpddsc, 
              p.prcnumsidoc, 
              num.spdnumero,
              num.spdid,
              to_char(num.spdddatainclusao, 'DD/MM/YYYY') as nuddatageracao,
              coalesce(ent.entnome,'ADMINISTRATIVO') as advogado,
              us.usunome,
              us.usucpf,
              co2.coonid,
              co2.coodsc
            FROM conjur.processoconjur p
            INNER JOIN conjur.sapiensdocumento num ON p.prcid  = num.prcid
            INNER JOIN conjur.tipodocumento       td ON num.tpdid = td.tpdid
            LEFT  JOIN conjur.sapiensanexo         a ON a.spdid = num.spdid AND a.spastatus = 'A'
            LEFT  JOIN conjur.advogados          adv ON adv.advid = num.advid
            inner JOIN    conjur.coordenacao co2 ON co2.coonid = adv.coonid
            LEFT  JOIN entidade.entidade         ent ON ent.entid = adv.entid
            LEFT  JOIN seguranca.usuario          us ON us.usucpf = num.usucpf
            WHERE 1=1 " . $where ." ");
  //     ver($sql,d);
    }
    $sqlParaExecutar = implode(' UNION ALL ', $sql);

    // ver($sqlParaExecutar,d);
    $lista = $db->carregar( $sqlParaExecutar );
    // ver($lista,d);
    if(empty($lista)){
        echo "<p style='margin-left: 30px;'>Não foi encontrado nenhum resultado</p>";die;
    }
    $arrayCoordenacoes = array();
    foreach ($lista as $key => $value) {
      if( $arrayCoordenacoes[ $value['coonid'] ][ $value['tpdid'] ]['contagem'] ){
        $arrayCoordenacoes[ $value['coonid'] ][ $value['tpdid'] ]['contagem']++;
      }else {
        $arrayCoordenacoes[ $value['coonid'] ]['coordenacao'] = $value['coodsc'];

        $arrayCoordenacoes[ $value['coonid'] ][ $value['tpdid'] ] = array();
        $arrayCoordenacoes[ $value['coonid'] ][ $value['tpdid'] ]['descricao'] = $value['tpddsc'];
        $arrayCoordenacoes[ $value['coonid'] ][ $value['tpdid'] ]['contagem'] = 1;
      }
    }
    // ver($arrayCoordenacoes,d);

    $array = array(
        'dados'=>array(),
        'alinhamento'=>array(),
        'cabecalho'=>array(),
        'params'=>array() );

    // dados
      foreach ($arrayCoordenacoes as $key => $value) {
        // ver($value,d);
        $total = 0;
        $arrayVez = array();
        $arrayVez['coordenacao'] = $value['coordenacao'];
        foreach ($arrayTPDID as $key2 => $value2) {
          $total = $total + $value[ $key2 ]['contagem'];
          $arrayVez[$value2] = ($value[ $key2 ]['contagem'])?$value[ $key2 ]['contagem']:0;
        }
        $arrayVez['total'] = $total;
        array_push($array['dados'],$arrayVez);
      }
      // ver($array,d);
    // --

    // restante
      $array['params']['totalLinhas'] = false;
      $array['alinhamento'] = array('','right','right','right','right','right','right','right');
      $array['cabecalho'] = array('Coordenação','Parecer', 'Cota', 'Nota','Despacho','Ofício','Informação','TOTAL');
    // --
    $db->monta_lista($array['dados'], $array['cabecalho'], '250','10', 'S', 'center', 'N', '', '', $array['alinhamento'], '', $array['params']);  
} 

function relatorioManifestacaoMEC($post){
    global $db;
    
    extract($post);

    $aryWhere[] = " num.spdstatus = 'A' ";

    $arrayTPDID = array(
          TP_PARECER => 'parecer',
          TP_COTA => 'cota',
          TP_NOTA => 'nota',
          TP_DESPACHO => 'despacho',
          TP_OFICIO => 'oficio',
          TP_INFORMACAO => 'informacao' );

    if($htddatainicial && $htddatfinal){
    	$aryWhere[] = "num.spdddatainclusao >= TIMESTAMP '{$htddatainicial}' AND num.spdddatainclusao <= TIMESTAMP '{$htddatfinal} 23:59:59'";
    }

//    $aryWhere[] = " adv.advid in ( SELECT a.advid
//          FROM conjur.advogados a
//          INNER JOIN entidade.entidade e ON e.entid=a.entid
//          WHERE entstatus='A' and a.advstatus = 'A' ) ";
//

    // inicio novo relatorio --
    $where = (is_array($aryWhere) ? ' AND '.implode(' AND ', $aryWhere) : '');

   $sql = array();
    foreach ($arrayTPDID as $key => $value) {
        $where = ( is_array($aryWhere) && count($aryWhere) ? ' and ' . implode(' AND ', $aryWhere) : '' );
        $where .= " and td.tpdid in ($key) ";

        array_push($sql, " SELECT distinct on (p.prcid)
              p.prcid,
              coalesce(a.spdid,0) as nudidanexo,
              a.arqid, 
              td.tpdid,
              td.tpddsc, 
              p.prcnumsidoc, 
              num.spdnumero,
              num.spdid,
              to_char(num.spdddatainclusao, 'DD/MM/YYYY') as nuddatageracao,
              coalesce(ent.entnome,'ADMINISTRATIVO') as advogado
            FROM conjur.processoconjur p
            INNER JOIN conjur.sapiensdocumento num ON p.prcid  = num.prcid
            INNER JOIN conjur.tipodocumento       td ON num.tpdid = td.tpdid
            LEFT  JOIN conjur.sapiensanexo               a ON a.spdid = num.spdid AND a.spastatus = 'A'
            LEFT  JOIN conjur.advogados          adv ON adv.advid = num.advid
            inner JOIN    conjur.coordenacao co2 ON co2.coonid = adv.coonid
            LEFT  JOIN entidade.entidade         ent ON ent.entid = adv.entid
            LEFT  JOIN seguranca.usuario          us ON us.usucpf = num.usucpf
            WHERE 1=1 " . $where ." ");
         //  ver($sql,d);
    }
    $sqlParaExecutar = implode(' UNION ALL ', $sql);

    // ver($sqlParaExecutar,d);
    $lista = $db->carregar( $sqlParaExecutar );
    // ver($lista,d);


    if(!empty($lista)) {
        $array = array(
            'dados' => array(),
            'alinhamento' => array(),
            'cabecalho' => array(),
            'params' => array());

        // dados
        $arrayAgrupador = array();
        foreach ($lista as $key => $value) {
            if ($arrayAgrupador[$key][$value['tpdid']]['contagem']) {
                $arrayAgrupador[$key][$value['tpdid']]['contagem']++;
            } else {

                $arrayAgrupador[$key][$value['tpdid']] = array();
                $arrayAgrupador[$key][$value['tpdid']]['descricao'] = $value['tpddsc'];
                $arrayAgrupador[$key][$value['tpdid']]['contagem'] = 1;
            }
        }
        // ver($arrayAgrupador,d);


        foreach ($arrayAgrupador as $key => $value) {
            // ver($value,d);
            $total = 0;
            $arrayVez = array();
            foreach ($arrayTPDID as $key2 => $value2) {
                $total = $total + $value[$key2]['contagem'];
                $arrayVez[$value2]['contagem'] = ($value[$key2]['contagem']) ? $value[$key2]['contagem'] : 0;
            }
            $arrayVez['total'] = $total;
            array_push($array['dados'], $arrayVez);
        }
        // ver($array,d);

        $arrayOrdem = array(
            'parecer' => 0,
            'cota' => 0,
            'nota' => 0,
            'despacho' => 0,
            'oficio' => 0,
            'informacao' => 0,
            'total' => 0);
        foreach ($array['dados'] as $key => $value) {
            foreach ($value as $key2 => $value2) {
                $arrayOrdem[$key2] = $arrayOrdem[$key2] + $value2['contagem'];
                $arrayOrdem['total'] = $arrayOrdem['total'] + $value2['contagem'];
            }
        }
        // ver($arrayOrdem,d);
        $array['dados'] = array($arrayOrdem);
        // ver($array,d);
        // --
        }
    // restante
      $array['params']['totalLinhas'] = false;
      $array['alinhamento'] = array('right','right','right','right','right','right','right','right');
      $array['cabecalho'] = array('Parecer', 'Cota', 'Nota','Despacho','Ofício','Informação','TOTAL');
    // --
    $db->monta_lista($array['dados'], $array['cabecalho'], '250','10', 'N', 'center', 'N', '', '', $array['alinhamento'], '', $array['params']);  

    // fim novo relatorio --
} 
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<?php  monta_titulo( 'Relatório de Manifestações Sapiens',''); ?>

<?php monta_titulo( '','Período:&nbsp;&nbsp;'.$_POST['htddatainicial'] . ' - ' .$_POST['htddatfinal'] ); ?>

<?php if($_POST['relA'] || $_POST['relT']){ ?>
  <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
  	<tr>
  		<td class="subtituloCentro" width="10%">Advogado</td>
  	</tr>
  </table>
  <div id="divManifestacao">
  	<?php relatorioManifestacao($_POST); ?>
  </div>
  <br>
<?php } ?>

<?php if($_POST['relC'] || $_POST['relT']){ ?>
  <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
  	<tr>
  		<td class="subtituloCentro" width="10%">Coordenação</td>
  	</tr>
  </table>
  <div id="divManifestacaoCoordenacao">
  	<?php relatorioManifestacaoCoordenacao($_POST); ?>
  </div>
  <br>
<?php } ?>

<?php if($_POST['relM'] || $_POST['relT']){ ?>
  <table align="center" class="listagem" cellspacing="1" cellpadding="1" width="95%">
  	<tr>
  		<td class="subtituloCentro" width="10%">MEC</td>
  	</tr>
  </table>
  <div id="divManifestacaoMEC">
  	<?php relatorioManifestacaoMEC($_POST); ?>
  </div>
<?php } ?>