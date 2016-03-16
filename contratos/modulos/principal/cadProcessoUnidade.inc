<?php
if( $_REQUEST['copid']){
	$_SESSION['copid'] = $_REQUEST['copid'];
}  
 
if( $_POST['action'] == 'grava'){
 	if( $_SESSION['copid']){
		salvar( $_SESSION['copid']); 
	}else{
		salvar(); 
	} 
}

verificaSessao(true);
 
if( $_SESSION['copid'] != ''){ 
	$sql = "SELECT 				 
				co.copnumprocesso,
				to_char(co.copdataabertura::date,'DD/MM/YYYY') AS copdataabertura,		 
				co.cooid, 		 
				co.cocid, 		 
				co.conid, 
				to_char(co.copdatalimite::date,'DD/MM/YYYY') AS copdatalimite,	
				co.usucpf,
				co.copdsc,
				us.usunome
			FROM 
				evento.coprocesso AS co 
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = co.usucpf 
			WHERE
				co.copid = '".$_SESSION['copid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql );
} 

function salvar($id = null){
	global $db; 
     
	$arrDatamI = explode( "/", $_REQUEST['copdataabertura']);
    $formataDatamA = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0];
                
    $arrDatamF = explode( "/", $_REQUEST['copdatalimite']);
    $formataDatamL = $arrDatamF[2].'-'.$arrDatamF[1].'-'.$arrDatamF[0];  
    
 	if( $id == NULL ){
 		 		 		
	    $sql = "INSERT INTO 
	    		evento.coprocesso
	    		(
				 copnumprocesso,
				 copdataabertura,		 
				 cooid, 		 
				 cocid, 		 
				 conid,   
				 copdatalimite,
				 usucpf,
				 copdsc
				)
				VALUES
				(
					'".$_POST['copnumprocesso']."',
					'{$formataDatamA}', 
					{$_POST['cooid']},
					{$_POST['cocid']},
					{$_POST['conid']},
					'{$formataDatamL}',
					'{$_SESSION['usucpf']}',
					'{$_POST['copdsc']}'
				) 
	    		RETURNING copid";
				 
 	}else{                    
		$sql = "UPDATE evento.coprocesso 
				SET
					copnumprocesso  = '".$_POST['copnumprocesso']."',
					copdataabertura = '$formataDatamA',
					cooid			= ".$_POST['cooid'].",
					cocid			= ".$_POST['cocid'].",
					conid 			= ".$_POST['conid'].",
					copdsc 			= ".$_POST['copdsc'].",
					copdatalimite 	= '$formataDatamL'
				WHERE
					copid = ".$_SESSION['copid']."
					RETURNING copid
		"; 
		
 	}
 	if ( $copid = $db->pegaUm( $sql ) ){
			$db->commit();
			$_SESSION['copid'] = $copid;
			echo"<script>window.location.href = 'evento.php?modulo=principal/cadProcesso&acao=A';</script>";
		
 	} 	
 	return true;
} 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoUnidade($_SESSION['unidade']); 
?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script>
    <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>
            <input type = "hidden" name="update" id="update" value="0">
            <input type = "hidden" name="insert" id="insert" value="0">
                <td class ="SubTituloDireita" align="right">Título Processo: </td> 
                <td> 
                     <?
					 $copdsc = $rsDadosProcesso[0]['copdsc'];
					 echo $copdsc;
                     ?>
            	</td>
            </tr>
        	<tr>
                <td class ="SubTituloDireita" align="right">Nº Processo: </td> 
                <td> 
                     <?
					 $copnumprocesso = $rsDadosProcesso[0]['copnumprocesso'];
					 echo $copnumprocesso;
                     ?>
            	</td>
            </tr>
           	<tr>
                <td class ="SubTituloDireita" align="right">Data de Abertura: </td>
                <td>  
                <?php 
                $copdataabertura  = $rsDadosProcesso[0]["copdataabertura"];   
                echo $copdataabertura;
                ?>
                 
                </td> 
            </tr>
            <tr>
                <td class="SubTituloDireita">Objetos:</td>
                <td align="left">
                <?php
                                 
                $sql = "SELECT 
                           cooid as codigo, 
                           coodsc as descricao
                        FROM 
                            evento.coobjeto
                        WHERE coostatus = 'A'";
                
                $sqlMarcados = "SELECT 
								cp.cooid as codigo ,
								c.coodsc as descricao
								FROM 
								evento.coprocessoobjeto AS cp
								INNER JOIN evento.coobjeto c ON cp.cooid = c.cooid
								WHERE cp.copid =  '{$_SESSION['copid']}'";
                           
				if( $_SESSION['copid'] != '' ){
					$cooid = $db->carregar( $sqlMarcados );
				}
                
                combo_popup( "cooid", $sql, "Selecione o(s) Objeto(s)", "192x400", 0, array(), "", "N", false, false, 5, 310 );
                ?>
                
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Tipo de Cotação: </td>
                <td> 
                <?php 
                $sql = "SELECT 
                            cocid AS codigo, 
                            cocdsc AS descricao
                        FROM
                            evento.cotipocotacao
                       ";
                             
                $cocid = $rsDadosProcesso[0]["cocid"];
                 
                $db->monta_combo('cocid', $sql, 'N', "Selecione...", '', '', '', '200', 'N', 'cocid');
                ?> 
              </td> 
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Nível da Contratação: </td>
               <td> 
                <?php 
                $sql = "SELECT 
                            conid AS codigo, 
                            condsc AS descricao
                        FROM
                            evento.conivelcontratacao
                       ";
                             
                $conid = $rsDadosProcesso[0]["conid"];
                 
                $db->monta_combo('conid', $sql, 'N', "Selecione...", '', '', '', '200', 'N', 'conid');
                ?> 
              </td> 
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Termo de referência: </td>
                <td> 
                <?php
				$sql = "SELECT 
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadCompraAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>'
                     FROM
                        ((public.arquivo arq INNER JOIN evento.coanexo aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.copid = " .$_SESSION['copid']."
                    AND tarq.tpaid = ".ID_TERMO_REFERENCIA."
                    LIMIT 1";
				if( $_SESSION['copid'] != '' ){
                	$termo = $db->pegaUm( $sql );
				}
				if( $termo ){
                	echo $termo;
                }else{
                	echo "Termo de referência não anexado";
                }
				?> 
                </td> 
            </tr>	
            <tr>
                <td class ="SubTituloDireita" align="right">Data limite para adesão: </td>
                <td>  
                <?php 
                $copdatalimite = $rsDadosProcesso[0]["copdatalimite"];   
                echo $copdatalimite;
                ?>
                </td> 
            </tr> 
         </form> 
      </table> 
<script type="text/javascript">
function validaForm(tipo)
{ 
	document.getElementById('action').value=tipo;
	document.formulario.submit();
	 
	if( tipo == 'cancel' )
	{
		window.location.href= "evento.php?modulo=inicio&acao=C";
	} 
}
</script>
 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
           <tr>
                <td class ="SubTituloEsquerda">Lista de Itens que compõem este processo </td>
           </tr>
</table>
<?php
$sql = "SELECT
			i.coicodsiasg, i.coidsc,
			um.umedescricao, 
			coalesce( i.coivlrreferenciamin, 0 ) / 1 as coivlrreferenciamin,
			coalesce( i.coivlrreferenciamax, 0 ) / 1 as coivlrreferenciamax
	    FROM evento.coitem i
			LEFT JOIN evento.unidademedida um on i.umeid = um.umeid
			INNER JOIN evento.coitemprocesso ip  on i.coiid = ip.coiid
			WHERE i.coistatus = 'A' and ip.copid = {$_SESSION['copid']} 
			and coiiditempai is null";
$cabecalho = array("Cód. SIASG", "Descrição", "Unidade de Medida", "Valor Min.", "Valor Máx." ); 
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '');
?>