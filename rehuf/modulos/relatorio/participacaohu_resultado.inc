<?php 

ini_set("memory_limit", "2048M");
$_REQUEST["titulo"] = "RELATÓRIO GERENCIAL DE PARTICIPAÇÃO DOS HUs SOBRE O TOTAL DE ITENS";

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup       = monta_agp();
$col         = monta_coluna();
$sql         = monta_sql();
$dadosPregao = $db->carregar($sql);
$entidades   =  lista_hospitais();
$dados       = array();  


/* Verifica se existe pregao e entidades */
if($dadosPregao[0] && $entidades[0])
{
  $styleItem = 'class="nitempregao" style="display:none;"';
  /* Loop de pregoes selecionados e concatencao jquery(style) */
  $i = 0;
  foreach($dadosPregao as $key1 => $pregao):
     $qtdItens = "<span {$styleItem}>".$pregao['nitempregao']."</span>".$pregao['itedescricao'];
     $situacao = $pregao['situacao'];
    
     /* Loop entidades */
     foreach ($entidades as $entidade) 
     {
        $ippquantidade = qtdItensprenchidos_HospitalxPregao($entidade['entid'], $pregao['preid'] );
      
        if($ippquantidade)
        {
            $hospabrv = '';
            $resumoent = explode(" ", $entidade['entnome']);
            $porcent = round(($ippquantidade*100)/$pregao['nitempregao'],1);
            for($j=0;$j<count($resumoent);$j++) { 
               $hospabrv .= strtoupper(substr(trim($resumoent[$j]),0,1)); 
            } 
            $hospital = (($entidade['entsig'])?$entidade['entsig']."-":"").$hospabrv.' / '.$entidade['entid'];
            $dados[$i]['preid'] =  $pregao['preid'];
            $dados[$i]['itedescricao'] =  $qtdItens;
            $dados[$i]['entnome'] =  $hospital;
            $dados[$i]['nitempregao'] =  "&nbsp;".$pregao['nitempregao'];
            $dados[$i]['nitempregaohu'] =  "&nbsp;{$ippquantidade}";
            $dados[$i]['porcitempregaohu'] =  "&nbsp;{$porcent}%";
            if($situacao == 2){
               $dados[$i]['statuspregao'] =  $entidade['esddsc'];
            }else{
               $dados[$i]['statuspregao'] =  "-";
            }
            $i++;        
        }
         
     }
    endforeach;  
}

// Monta agrupadores
function monta_agp()
{
	$agrupador = array(0 => "itedescricao", 
                           1 => "entnome", 
                           2 => "nitempregao", 
                           3 => "nitempregaohu", 
                           4 => "porcitempregaohu",
                           5 => "statuspregao"
                          );
        $_REQUEST['agrupador'] = $agrupador;
	
	$agp = array(
        "agrupador" => array( 
                             array('campo' => 'itedescricao', 'label' => 'Pregao'), 
                             array('campo' => 'entnome', 'label' => 'hu')),
            
        "agrupadoColuna" => array(
            "itedescricao",
            "entnome",
            "nitempregao",
            "nitempregaohu",
            "porcitempregaohu",
            "statuspregao"
        )
    );
   
    return $agp;
}

// Monta colunas conforme agrupadores
function monta_coluna()
{
     $coluna = array();
     $coluna[] = array("campo" => "nitempregao", "label" => "Nº Itens Pregão");
     $coluna[] = array("campo" => "nitempregaohu", "label" => "Nº Itens Preenchidos por HU");
     $coluna[] = array("campo" => "porcitempregaohu", "label" => "% Itens Preenchidos por HU");
     $coluna[] = array("campo" => "statuspregao", "label" => "Status");
   
     return $coluna;	
}

// Monta sql do relatorio
function monta_sql($retornaQuery = NULL)
{
	global $db;
        $arWhere = array();
	extract($_REQUEST);
	
        
        $arWhere[] = " g.grustatus = 'A' ";
        $arWhere[] = " ig.itgstatus = 'A' ";
        $arWhere[] = " igp.igpstatus = 'A' ";
        $arWhere[] = " pre.prestatus = 'A' ";
        
        if($preid[0]){
           $preid = implode(",",$preid);
           $arWhere[] = " igp.preid in ($preid) ";
        }
        
        
        
        if($preinsinicial || $preinsfinal){		
		if($preinsinicial && $preinsfinal){
			$arWhere[] = " pre.predatainicialpreenchimento::date between '".formata_data_sql($preinsinicial)."' and '".formata_data_sql($preinsfinal)."' ";
		}else if($preinsinicial){
			$arWhere[] = "to_char(pre.predatainicialpreenchimento,'YYYY-MM-DD') = '".formata_data_sql($preinsinicial)."' ";
		}else if($preinsfinal){
			$arWhere[] = " to_char(pre.predatainicialpreenchimento,'YYYY-MM-DD') = '".formata_data_sql($preinsfinal)."' ";
		}
	}

        $sql = "SELECT  count(1) as nitempregao,
                        igp.preid,
                        pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')' as itedescricao,
                        rehuf.f_pregao_situacao_preenchimento(pre.predatainicialpreenchimento, pre.predatafinalpreenchimento) as situacao
                  FROM 
                        rehuf.item i 
			inner join rehuf.itemgrupo ig 
			  	on (i.iteid = ig.iteid) 
			inner join rehuf.grupoitens g
			  	on (ig.gruid = g.gruid)
			inner join rehuf.itemgrupopregao igp
    			        on (ig.itgid = igp.itgid) 
                        inner join rehuf.pregao pre
                                on pre.preid = igp.preid
    		  left join rehuf.itemlote il 
    		  	on (il.itlid = i.itlid)
			WHERE i.itestatus = 'A'
			AND g.grustatus = 'A'
			AND ig.itgstatus = 'A'
			AND igp.igpstatus = 'A'
			".( !empty($arWhere) ? ' AND' . implode(' AND ', $arWhere) : '' ).
			" GROUP BY igp.preid, pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')', rehuf.f_pregao_situacao_preenchimento(pre.predatainicialpreenchimento, pre.predatafinalpreenchimento)  
                         ORDER BY igp.preid, pre.preobjeto||' ('||COALESCE(pre.precodigo,'-')||')'";
         $itens = $db->carregar($sql);
         
         if($retornaQuery){
           return $arWhere;  
         }
         
	return $sql;
}

function lista_hospitais()
{
    global $db;
    extract($_REQUEST);
    if($entid[0]){
         $entid = implode(",",$entid);
         $entidade = " AND ent.entid in ($entid) ";
     }else{
         $entidade = '';
     }
    
    $sql = "SELECT ent.entnome, ena.entsig, ent.entid, esd.esddsc FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
		LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
                INNER JOIN workflow.documento doc ON doc.docid = esu.docid 
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		WHERE fen.funid IN ('". HOSPITALUNIV ."','".HOSPITALFEDE."') 
                AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
                {$entidade}
		ORDER BY ena.entsig";
   
    return  $db->carregar($sql);
}

function qtdItensprenchidos_HospitalxPregao( $entid, $preid)
{
    global $db;
    $sql = "SELECT count(1) FROM rehuf.itenspregaopreenchido ipp 
	    INNER JOIN rehuf.huparticipante hup ON hup.hupid=ipp.hupid 
	    WHERE entid='".$entid."' AND preid='".$preid."' AND ippquantidadeanual is not null";
  
    return $db->pegaUm($sql);
}


 /* Visualizar */
if($_REQUEST['buscar'] == 1){ ?>
<html>

	<head>
	
		<title>RELATÓRIO GERENCIAL DE PARTICIPAÇÃO DOS HUs SOBRE O TOTAL DE ITENS - REHUF</title>
                <script type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
                <style type="text/css" media="print">
                    @media print{
                        *{
                            margin:0;
                            padding:0;
                        }
                        .LandscapeDiv{
                            width: 100%;
                            height: 100%;
                            filter: progid:DXImageTransform.Microsoft.BasicImage(Rotation=3);
                        }
                    }
                </style>
 	</head>
        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0" class="LandscapeDiv">	
          	<?php
			$r = new montaRelatorio();
			$r->setAgrupador($agrup, $dados); 
			$r->setColuna($col);
                        $r->setTotNivel(true);
			$r->setMonstrarTolizadorNivel(false);
			$r->setBrasao(true);
			$r->setEspandir(false);
			$r->setTotalizador(false);
                        echo $r->getRelatorio();
		
                 if($dadosPregao[0] && $entidades[0]){ ?>
                  <br/><div class="notprint" align="center">
                     <input type="button" value="Imprimir" 
                         class="notprint" style="cursor: pointer, info{ display: none; }" onclick="imprimirRel()">
                 </div>
                 <script type="text/javascript">
                     $(document).ready(function(){
                        $('.nitempregao').each(function(){
                            $(this).parent().parent().next().text($(this).text());
                            $(this).parent().parent().next().next().html('&nbsp;');
                            $(this).parent().parent().next().next().next().html('&nbsp;');
                            $(this).parent().parent().next().next().next().next().html('&nbsp;');
                          });
                          $('.tabela tr:last').remove();
                          $('.tabela').attr('border','1px solid black');
                     });
                   
                 function imprimirRel(){
                    $('img[id^="itedescricao_"]').each(function(){
                       if($(this).attr('src') ==  "/imagens/mais.gif"){
                          $(this).click(); 
                        }
                      });
                    setTimeout(function(){
                        self.print();
                    }, 800);
                 }
                </script>    
             </body> 
</html> 
        <?php }/* IF botao Imprimir */
           
           /* Gerar excel */
           }elseif($_REQUEST['buscar'] == 2){
             
              $arrColuna = array(0 => "Pregão", 1 => "HU");
               foreach($col as $coluna):
                   $arrColuna[] = $coluna['label'];
                endforeach; ?>

    <table border="1">
                    <tr>
                        <?php foreach($arrColuna as $coluna):
                              echo "<td bgcolor='#F2F2F2'>".$coluna."</td>";  
                             endforeach;
                             $i =0;
                             $arrNumero = array("0","1","2","3","4","5","6","7","8","9");
                             $pregaoAntigo = '';
                             foreach($dados as $key => $linha):
                               $qtd = strlen($linha['itedescricao']);
                               $span = str_replace('<span class="nitempregao" style="display:none;">', '', $linha['itedescricao']);
                               $pregao = str_replace('</span>', '', $span);
                               $qtd =  strlen($pregao);
                               for($j=0;$j<$qtd;$j++){
                                 $letra = substr($pregao,$j,1);
                                 if(!in_array($letra,$arrNumero)){
                                     break;
                                 }
                               }
                               $preid = substr($pregao,0,$j);
                               $pregao = substr($pregao,$j, $qtd);
                               if($pregao != $pregaoAntigo && $pregaoAntigo != ''){
                                  echo "<tr>";
                                  foreach($arrColuna as $coluna):
                                    echo "<td bgcolor='#F2F2F2'>".$coluna."</td>";  
                                  endforeach;
                                  echo "</tr>"; 
                               }
                               $pregaoAntigo = $pregao;
                               echo "<tr><td>".$pregao."</td>";     
                               echo "<td>".$linha['entnome']."</td>";     
                               echo "<td>".$preid."</td>";     
                               echo "<td>".$linha['nitempregaohu']."</td>";     
                               echo "<td>".$linha['porcitempregaohu']."</td>";     
                               echo "<td>".$linha['statuspregao']."</td></tr>";     
                             endforeach;
                        ?>
                    </tr>
          </table>
  
         <?php
               header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	       header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
               header ( "Pragma: no-cache" );
     	       header ( "Content-type: application/xls; name=REHUF_RelatorioParticipantes_".date("Ymdhis").".xls");
	       header ( "Content-Disposition: attachment; filename=REHUF_RelatorioParticipantes_".date("Ymdhis").".xls");
	       header ( "Content-Description: MID Gera excel" );
               
             }
           ?>

