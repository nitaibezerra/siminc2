<?php 
//Teste XLS
if($_POST['limpaSession']){
	unset($_SESSION['evento']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['evento']['post']) ){
	$_SESSION['evento']['post'] = $_POST;
}

//Rececendo os valores dos filtros
extract($_SESSION['evento']['post']);
// Contrato
//if( is_array($tipoContrato) ){
if( $tipoContrato[0] != '' && ( $tipoContrato_campo_flag || $tipoContrato_campo_flag == '1' ) ){
    $where[] = " iq.coeid IN ('".implode("','",$tipoContrato)."') ";
}
//if( is_array($porteEvento) && $porteEvento[0] ){
//if( $porteEvento[0] && ( $porteEvento_campo_flag || $porteEvento_campo_flag == '1' ) ){
//    //porminpart ||' até '|| pormaxpart as descricao
//    $where[] = " p.porid IN ('".implode("','",$porteEvento)."') ";
//}
///////////////////////////////////////////////////////////SELECT DA CONSULTA DO RELATÓRIO GERAL DE CONTRATOS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(is_array($where)){
	if(count($where)> 0){
		$listaWhrew = " AND " . implode( " AND ", $where );
	}else{
		$listaWhrew = '';
	}
}

$sql = "SELECT 
            i.iteitem || ' ' as id, 
            pai.iteespecificacao||' -- '||i.iteespecificacao ||' -- '||g.grudescricao as item_descricao, 
            umedescricao,
            
            CASE WHEN iq.iqtqtdmax IS NULL or iq.iqtqtdmax = 0 
                THEN  0 
                ELSE iq.iqtqtdmax 
            END quantidade_contratada, 
            
            --itvvalor,
            
            CASE WHEN iv.quantidade_utilizada IS NULL or iv.quantidade_utilizada = 0 
                THEN  0 
                ELSE iv.quantidade_utilizada 
            END quantidade_utilizada, 
            
            CASE WHEN ivv.quantidade_comprometida IS NULL or ivv.quantidade_comprometida = 0 
                THEN 0 
                ELSE ivv.quantidade_comprometida 
            END quantidade_comprometida,
            
            COALESCE(iv.quantidade_utilizada + ivv.quantidade_comprometida,0) as quantidade_consolidada,  
            COALESCE(iq.iqtqtdmax - (iv.quantidade_utilizada + ivv.quantidade_comprometida),0) as quantidade_consolidada_disponivel,
            
            CASE WHEN iv.quantidade_utilizada = 0 or iv.quantidade_utilizada IS NULL 
                THEN iq.iqtqtdmax 
                    WHEN iq.iqtqtdmax  = 0 or iq.iqtqtdmax IS NULL 
                        THEN iv.quantidade_utilizada
                ELSE (iq.iqtqtdmax - iv.quantidade_utilizada) 
            END quantidade_disponivel
            
        FROM evento.item i

        JOIN evento.unidademedida uni ON uni.umeid = i.umeid

        JOIN evento.itemquantidade iq ON iq.iteid = i.iteid

        JOIN evento.grupo g ON g.gruid = iq.gruid

        JOIN evento.contratopregao cp ON cp.coeid = iq.coeid

        LEFT JOIN(
                SELECT itv.iteid, itv.coeid, sum(itc.itcquantidade) AS quantidade_utilizada 
                FROM evento.itemvalor AS itv
                JOIN evento.itemconsumo AS itc ON itc.itvid = itv.itvid

		JOIN evento.evento AS ev ON ev.eveid = itc.eveid
		JOIN workflow.documento d ON d.docid = ev.docid
		JOIN workflow.estadodocumento est ON est.esdid = d.esdid
	        WHERE est.esdid <> ".EM_CADASTRAMENTO_WF."
                    
                GROUP BY itv.iteid, itv.coeid
        ) AS iv ON iv.iteid = i.iteid AND iv.coeid = iq.coeid

        LEFT JOIN(
                SELECT itv.iteid, itv.coeid, sum(itc.itcquantidade) AS quantidade_comprometida
                FROM evento.itemvalor AS itv
                JOIN evento.itemconsumo AS itc ON itc.itvid = itv.itvid

		JOIN evento.evento AS ev ON ev.eveid = itc.eveid
		JOIN workflow.documento d ON d.docid = ev.docid
		JOIN workflow.estadodocumento est ON est.esdid = d.esdid
                WHERE est.esdid = ".EM_CADASTRAMENTO_WF."
                
                GROUP BY itv.iteid, itv.coeid
        ) AS ivv ON ivv.iteid = i.iteid AND ivv.coeid = iq.coeid
        

        JOIN(
                SELECT iteid, iteidpai, iteespecificacao FROM evento.item
        ) as pai on pai.iteid = i.iteidpai

        
        WHERE 1 = 1 
        
       ".$listaWhrew."
           
        GROUP BY  
                i.iteid,
                i.iteitem, 
                g.grudescricao, 
                i.iteespecificacao, 
                iqtqtdmax, 
                pai.iteespecificacao, 
                umedescricao,
                iv.quantidade_utilizada,
                ivv.quantidade_comprometida 
        ORDER BY 
                coalesce(split_part(i.iteitem, '.', 1),'0')::numeric, 
                coalesce(split_part(i.iteitem, '.', 2),'0')::numeric, 
                case when split_part(i.iteitem, '.', 3)='' then '0' else split_part(i.iteitem, '.', 3) end::numeric

        "
    ;
    $cabecalho = array("N° do Item","Descrição do Item","Unidade de medida","Quantidade Contratada","Quantidade Utilizada","Quantidade Comprometida","Quantidade Consolidada","Quantidade Consolidada Disponível","Quantidade Disponível");

//    ver($sql,d);
    
    if($_REQUEST['consulta'] == 'xls'){
        header('content-type: text/html; charset=ISO-8859-1');
        $db->sql_to_excel($sql, 'RelatorioContratos', $cabecalho);
        echo "<script>window.parent.close();</script>";
        exit;
    }
    $arDados = $db->carregar( $sql );
?>

<html>
    <head>
        <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
    
<table style="width:100%;">
	<tr>	
		<td>
		<?php 
                global $db;
                    if(!empty($_POST['tipoContrato'][0])){
                        $sqlContratos = "SELECT
                                                cp.coerazaosocial 
                                         FROM 
                                                evento.contratopregao AS cp
                                         WHERE cp.coestatus = 'A'
                                         AND cp.coeid IN (".implode(",",($_POST['tipoContrato'])).")
                                         -- ORDER BY cp.coeid ASC ";
                        $coerazaosocial = $db->carregar($sqlContratos);
                        if(!empty($coerazaosocial)){
                            foreach($coerazaosocial as $razaosocial){
                                $strrazaosocial .= $razaosocial['coerazaosocial']." - ";
                            }
                        }
                    }
		echo "<br/>";
		$cabecalhoBrasao = monta_cabecalho_relatorio('100');
		echo $cabecalhoBrasao;
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none; width:100%;">';
                $strrazaosocial == '' ? $strrazaosocial = "Todos  " : $strrazaosocial;
		monta_titulo( 'Relatório Quantitativo Itens por Contrato', substr($strrazaosocial, 0, -2) );
		print '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >&nbsp;</td></tr></table>';
		?>		
		</td>
	</tr>
	<tr>
		<td id="listaArray">
		<?php 
		$db->monta_lista_array($arDados, $cabecalho, 1000000, 1, 'N', 'center', '');													
		?>		
		</td>
	</tr>
</table>
<script>
var td = document.getElementById('listaArray');
var table = td.getElementsByTagName('table')[1];
table.style.width = '100%';
</script>
</body>
</html>