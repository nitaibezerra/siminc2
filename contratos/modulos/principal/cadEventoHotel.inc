<?php
error_reporting(0);
ini_set('display_errors', FALSE); 
 
$docid = evtCriarDoc();

if( $_REQUEST['excluir'])
{
	$sql = "DELETE FROM evento.hotelevento WHERE hoeid = '{$_REQUEST['excluir']}'";
}

if( $_POST['ajaxestuf'] ) 
{	 
	header('content-type: text/html; charset=ISO-8859-1');
 
	$sql = "select
			 muncod as codigo, mundescricao as descricao 
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";
	die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '', '', 'muncod' ));
}

if( $_POST['ajaxitem'] )
{
	$sql = "SELECT
                			it.tieid, 
                			it.tieitem,
                			it.tiedescricao,
                			it.tiedetalhe,
                			it.umeid,
                			it.tievalordf,
                			it.tievaloroutrauf,
                			it.tieidpai,
                			it.tieitemtemvalor,
                			it.tieitemtemcotacao,
                			it.tiastatus,
                			un.umedescricao
                		FROM 	
                			evento.tipoitemevento AS it
                		INNER JOIN
                			evento.tunidademedida AS un ON it.umeid = un.umeid
                		WHERE tieid = '{$_POST['ajaxitem']}'
                		 ";
	
	$rsAjax = $db->carregar( $sql );
	
	$resp = $rsAjax[0]['umedescricao'].'_'.$rsAjax[0]['tievalordf'].'_'.$rsAjax[0]['tievaloroutrauf'].'_'.$rsAjax[0]['tiedescricao'].'_'.$rsAjax[0]['tieitemtemcotacao'];
	echo $resp;
	die();
} 

if( $_POST['action'] == 'grava' ){
	salva();
}

function salva() {
	global $db;
 
	if(!$_REQUEST['alterar'])
	{
		$sql = "
				INSERT INTO evento.hotelevento
					(
						eveid,
						hoenome,
						muncod,
						estuf,
						clhid
					)
				VALUES
					(
						'{$_SESSION['eveid']}',
						'{$_POST['hoenome']}',
						'{$_POST['muncod']}',
						'{$_POST['estuf']}',
						'{$_POST['clhid']}'
					) 
					RETURNING hoeid
				";
	}
	else
	{
		$sql = "UPDATE 
					evento.hotelevento
				SET 
					hoenome = '{$_POST['hoenome']}',
					muncod = '{$_POST['muncod']}',
					estuf = '{$_POST['estuf']}',
					clhid = '{$_POST['clhid']}'
				WHERE
					hoeid = '{$_GET['alterar']}'
				RETURNING 
					hoeid
					";			
	}
	$hoeid = $db->pegaUm( $sql );
	$db->commit();
	
	$sqlDel = "DELETE FROM evento.capacidadehospedagem WHERE hoeid = '$hoeid' ";
	$db->executar( $sqlDel);
	$db->commit(); 
	
	for( $i = 0; $i < count($_POST['tpHospId']); $i++)
	{
		if( $_POST['txtQtd'][$i] == '')
		{
			$txtQtd = 0;
		}
		else
		{
			$txtQtd = $_POST['txtQtd'][$i];
		}
		
		$sql = "
			INSERT INTO evento.capacidadehospedagem
				(
					hoeid,
					tihid,
					cahquantidadehospedagem 
				)
			VALUES
				(					
					$hoeid,
					{$_POST['tpHospId'][$i]},
					'$txtQtd' 
				)										
		"; 	
		$ins = $db->executar( $sql );
		$db->commit();
		
	}
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
} 

$titulo_modulo = "Eventos";

if( $_SESSION['eveid'] != '')
{ 
	$sql = "SELECT 
				ev.evetitulo,  
				ev.ungcod,
				ev.tpeid,			 
				ev.evedatainicio,		 
				ev.evedatafim,			 
				ev.eveemail, 		 
				ev.evenumeropi, 		 
				ev.evenumeroprocesso,   
				ev.evecustoprevisto, 
				ev.evequantidadedias,	 
				ev.evepublicoestimado,  
				ev.muncod, 				 
				ev.estuf, 			
				ev.eveurgente,	 
				ev.sevid,
				u.ungdsc,
				ev.docid,
       			us.usunome,
				to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao			 
			FROM 
				evento.evento AS ev
			LEFT JOIN evento.tipoevento AS te 
				ON te.tpeid = ev.tpeid
			LEFT JOIN 
				public.unidadegestora AS u ON ev.ungcod = u.ungcod
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
			WHERE
				ev.eveid = '".$_SESSION['eveid']."'";
	
	$rsDadosEvento = $db->carregar( $sql );
	 
}

if( $_REQUEST['alterar'] != '')
{
	$sql = "SELECT
				h.hoeid,
				h.hoenome,
				h.muncod,
				h.estuf,
				h.clhid
			FROM 
				evento.hotelevento AS h
			WHERE
				h.hoeid = '{$_REQUEST['alterar']}'
	 			";
	$rsHotel = $db->carregar( $sql );
}	

 
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
echo'<br>';

?>
<br> 
<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<?

 $res = array(0 => array ( "descricao" => "Lista",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  ),
 				1 => array ( "descricao" => "Informações Básicas",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=principal/cadEvento&acao=A"
				  		  )
			);	  
				
if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res,
					array ("descricao" => "Documentos Anexos",
							    "id"        => "3",
							    "link" 		=> "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
							   ),
		   			array ("descricao" => "Infraestrutura",
							    "id"		=> "2",
							    "link"		=> "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
					  		   )
			  );
			  	$pflcod = pegaPerfil( $_SESSION['usucpf'] ); 
	 if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
	 { 
			array_push($res,	
					array ("descricao" => "Hotéis",
						    "id"		=> "1",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
				  		   ) 
				    );
	 }
	array_push($res,	
					 
				  	array ("descricao" => "Avaliação",
						    "id"		=> "0",
						    "link"		=> "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
				  		   )
					  );
}

if(mostraAbaDocPagamento($_SESSION['eveid'])){
		array_push($res,	
				  	array ("descricao" => "Documento de Pagamento",
						    "id"		=> "5",
						    "link"		=> "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
				  		   )
					  );
}
/* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
*
if($_SESSION['eveid']){
	array_push($res,	
				  	array ("descricao" => "Estrutura Orçamentária",
						    "id"		=> "6",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
				  		   )
					  );	
}
*/

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A");
headEvento($rsDadosEvento[0]['evetitulo'],$rsDadosEvento[0]['usunome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao']);
echo '<br>';


//verifica permissão edição
$eventoPermissaoEdicao = eventoPermissaoEdicao();

 ?>
 
  <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

            <tr>
                <td class ="SubTituloDireita" align="right">Nome do Hotel: </td>
                <td colspan="2"> 
      				 <?
					 $hoenome = $rsHotel[0]['hoenome'];
                     ?>
            		 <?= campo_texto('hoenome', 'S', $somenteLeitura, '', 60, 100, '', '', 'left', '',  0, 'id="hoenome" onblur="MouseBlur(this);"' ); ?>
               
                </td> 
                <td rowspan="6" style="background-color:#fafafa; color:#404040; width:1%" valign="top" align="left">
				<?php 
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( '' => '' ) );			
				?>		
				</td>  
            </tr>    
			<tr>
                <td class ="SubTituloDireita" align="right">UF: </td>
                <td colspan="2"> 
                    
                <?php
                	 $estuf = $rsHotel[0]['estuf'];
					$sql = "SELECT estuf as codigo, estdescricao as descricao from territorios.estado order by estdescricao";
					
            	?>
            	 <?=$db->monta_combo('estuf', $sql, 'S', "Selecione...", 'filtraMunicipio', '', '', '100', 'S', 'estuf');?>
             
                </td>
                 
            </tr>
            <tr>
                <td  class ="SubTituloDireita" align="right">Município: </td>
                <td id="municipio" colspan="2"> 
                    
           		<?php
           		$muncod = $rsHotel[0]['muncod'];
           			 
           			$sql = "select
								 muncod as codigo, 
								 mundescricao as descricao 
								from
								 territorios.municipio 
								order by
								 mundescricao asc";
 
            	  $db->monta_combo('muncod', $sql,'S', "Selecione...", '', '', '', '200', 'S', 'muncod');
            	  ?>
                </td>
            </tr>
			<tr>
                <td  class ="SubTituloDireita" align="right">Classificação: </td>
                <td id="classificacao" colspan="2"> 
                    
           		<?php
           			$clhid  = $rsHotel[0]["clhid"]; 
           			$sql = "SELECT
								 clhid as codigo, 
								 clhdescricao as descricao 
								FROM
								 evento.classificacaohotel
								order by
								 clhdescricao asc";
           		 
            	  $db->monta_combo('clhid', $sql,'S', "Selecione...", '', '', '', '200', 'S', 'clhid');
            	  ?> 
                </td> 
            </tr> 
     		<tr>
                <td class ="SubTituloDireita" align="right">Capacidade de Hospedagem: </td>
                <td colspan="2">
                <table width="400px" id="tbTipos" >
                <tr>
	                <th width="90%">
	                	Tipo
	                </th>
	                <th width="10%">
	                	Qtd
	                </th>   
                </tr>
           		<?php
           			$sql = "SELECT
							 th.tihid,
							 th.tihdescricao,
							 th.tihquantidadepessoas 
 
							FROM
							 evento.tipohospedagem AS th
							ORDER BY tihid
							 
								 ";

           			$rsTipoHospedagem = $db->carregar( $sql );

           			
           			$tstQtd = array();
           			//$rsCap = array();
           			$k = 0;
           			for( $i = 0; $i <count( $rsTipoHospedagem ); $i++)
           			{
           				if($rsHotel[0]['hoeid'] != '')
						{
	           				$sqlcap = "SELECT 
	           							c.cahid,
	           							c.cahquantidadehospedagem
	           						FROM
	           							evento.capacidadehospedagem AS c 
	           						WHERE
	           							tihid = '{$rsTipoHospedagem[$i]['tihid']}'
		 							AND 
	           							hoeid = '{$rsHotel[0]['hoeid']}'";
							$rsCap = $db->carregar($sqlcap);
							
						}
 					 
           				if( $rsTipoHospedagem[$i]['tihquantidadepessoas'] != '' )
           				{
           					if($rsTipoHospedagem[$i]['tihquantidadepessoas'] > 1)
           					{
           						$s = 's';
           					}
           					else
           					{
           						$s = '';
           					}
           					$pessoas = " - ".$rsTipoHospedagem[$i]['tihquantidadepessoas']. ' Pessoa';
           				}
           				else
           				{
           					$s = '';
           					$pessoas = '';
           				} 
           			?> 
           			<tr>
						<td id="capacidade[<?=$i;?>]" width="90%">  
							<?echo $rsTipoHospedagem[$i]['tihdescricao'].$pessoas.$s; ?>
							<input type="hidden" name="tpHospId[<?=$i;?>]" value="<?= $rsTipoHospedagem[$i]['tihid']; ?>" id="tpHospId[<?=$i;?>]">
		                </td> 
		                <td id = "qtd[<?=$i;?>]" align="center" width="10%">

	                		<input type = "text" onkeyup="calcula();" onblur="calcula();" maxlength="1" name="txtQtd[<?=$i?>]" size="3" id = "txtQtd[<?=$i?>]" class="normal" value="<?=$rsCap[0]['cahquantidadehospedagem']; ?>">
						</td>

	           		</tr>	
           			<?
           			$k++;
           			}
           			?>
           			<tr style="background-color: #E5E9E8;" align="right">
           				<td align="right" width="90%">
           					<b> Total: </b> 
           				</td>
           				<td align="center" id="total" width="10%">
           					 
           				</td>           				
           			</tr>		
           			</table>
           		</td>
            </tr> 
           	<? if( !temPerfilEmpresa() ){ ?>  
           	<tr>
            	 <td class ="SubTituloDireita" align="right">  &nbsp;
            	 </td>
            	 <td colspan="2">
	               <input type="hidden" name="action" value="0" id="action">
	               <input type="hidden" name="hoeid" value="<?=$rsHotel[0]['hoeid']; ?>" id="hoeid">
	               <input type="hidden" name="tamHospedagem" value="<?=count( $rsTipoHospedagem ); ?>" id="tamHospedagem">
	               <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">  
	               <input type="button" name="btnCancel" value="Cancelar" id="btnCancel" onclick="validaForm('cancel');">  
            	 </td> 
            </tr>	
            <? } ?>
            <table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?
    
 		$acao = "'<center><a href=\"#\" onclick=\"javascript:alterarHotel('|| h.hoeid || ');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"></a>&nbsp;<a href=\"#\" onclick=\"javascript:excluirHotel('|| h.hoeid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'"; 
    	if($eventoPermissaoEdicao) $acao = "''";
    
        $sql = "SELECT
                        $acao as acao,                       
                        h.hoenome,
                        es.estuf,
                        m.mundescricao,
                        cla.clhdescricao
                        --,
                        --'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadEventoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        --arq.arqtamanho || ' kbs' as tamanho ,
                        
                    FROM
						evento.hotelevento AS h
					INNER JOIN territorios.estado AS es ON es.estuf = h.estuf
					INNER JOIN territorios.municipio AS m ON m.muncod = h.muncod
					INNER JOIN evento.classificacaohotel AS cla ON cla.clhid = h.clhid
                    WHERE
                       h.eveid = " .$_SESSION['eveid'];

        $cabecalho = array( "Ação",
                            "Hotel",
                            "UF",
                            "Município",
                            "Classificação");
        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
    ?>
</table>
</table>
</form>
<script>

function validaForm(tipo)
{ 
	if( tipo == 'grava' )
	{	
		<?
		if( count($rsTipoHospedagem) > 0 )
		{
		?>
		var nHosp   = <?=count($rsTipoHospedagem); ?>;
		<?
		}
		?>
		var hoenome = document.getElementById('hoenome');  
		var muncod  = document.getElementById('muncod');  
		var estuf   = document.getElementById('estuf');  
		var clhid	= document.getElementById('clhid');  
		var txtQtd  = document.getElementById('txtQtd');
		
		if( hoenome.value == '')
		{
			alert('É necessário informar um nome para o Hotel.');
			return false;
		}
		if( muncod.value == '')
		{
			alert('É necessário informar um município.');
			return false;
		}
		if( estuf.value == '')
		{
			alert('É necessário informar um estado.');
			return false;
		}
		if( clhid.value == '')
		{
			alert('É necessário informar uma classificação para o hotel.');
			return false;
		} 
		document.getElementById('action').value = tipo;
		document.formulario.submit();	
	}
}
 
function calcula()
{ 
 	 <?
	 if( count( $rsTipoHospedagem ) != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= count( $rsTipoHospedagem ); ?>);
		 <?
	 }
 	 ?>	
	 var qtdHospedagem = 0;
	 var qtdHospedagemTotal = 0; 
	 var qtdTotalHospedagem = 0;
	 var qtd;
	 for( var i= 0; i< numCampos; i++ )
	 {
	 	 qtdHospedagem       = document.getElementById('txtQtd['+i+']');
 		 qtd = Number( qtdHospedagem.value );
	 	 qtdHospedagemTotal += Number(qtd);  
	 	 
	 	 if( isNaN( qtdHospedagemTotal ) )
	  	 { 
	  	 	 qtdTotalHospedagem = document.getElementById('total');
		 	 qtdTotalHospedagem.innerHTML = '';
		 }
		 else
		 {
		 	 qtdTotalHospedagem = document.getElementById('total');
		 	 qtdTotalHospedagem.innerHTML = qtdHospedagemTotal;
		 }		
	 } 
	  
} 

 
 function float2moeda(num) {
   x = 0;
   if(num<0) {
      num = Math.abs(num);
      x = 1;
   }
   if(isNaN(num)) num = "0";
      cents = Math.floor((num*100+0.5)%100);
   num = Math.floor((num*100+0.5)/100).toString();
   if(cents < 10) cents = "0" + cents;
      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
         num = num.substring(0,num.length-(4*i+3))+'.'
               +num.substring(num.length-(4*i+3));
    ret = num + ',' + cents;
    if (x == 1) ret = ' - ' + ret;
    return ret;
}

function filtraMunicipio(estuf)
{
	//alert( estuf);
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('evento.php?modulo=principal/cadEvento&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,							         
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
										td.style.visibility = 'visible';
							        }
							        
							  });
    
}

function excluirHotel(id)
{	
	if( id != '')
	{
		return location.href='evento.php?modulo=principal/cadEventoHotel&acao=A&excluir='+id; 
	}
}
function alterarHotel(id)
{	
	if( id != '')
	{
		return location.href='evento.php?modulo=principal/cadEventoHotel&acao=A&alterar='+id; 
	}
}


calcula();
 </script>
 
 
<?php 
//verifica permissao edição
echo $eventoPermissaoEdicao;
?>