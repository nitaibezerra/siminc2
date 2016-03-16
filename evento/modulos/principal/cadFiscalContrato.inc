<?php
include_once APPRAIZ . "includes/classes/dateTime.inc";
 
//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();


function salvar(){
	global $db; 
 	 
	$formataDatax = formata_data_sql( $_REQUEST['ctdataportfiscal'] );
	
	$sql = "UPDATE evento.ctcontrato 
			SET
				ctnumportfiscal		= ".$_POST['ctnumportfiscal'].",
				ctdataportfiscal	= '".$formataDatax."',
				entidfiscaltit		= '".$_POST['entidfiscaltit']."',
				entidfiscalsub		= '".$_POST['entidfiscalsub']."'
			WHERE
				ctrid = ".$_SESSION['ctrid']."
				RETURNING ctrid
	"; 

	$ctrid = $db->pegaUm($sql);
	$db->commit();
	$_REQUEST['acao'] = "A";
	$db->sucesso('principal/cadFiscalContrato', "");
	die;
} 


if($_POST['action']){
	salvar();
}


			
$sql = "SELECT
				ent.entnome as usunometit, 
				ent2.entnome as usunomesubst, 
				c.ctnumportfiscal as ctnumportfiscal,
				c.ctdataportfiscal as ctdataportfiscal,
				c.entidfiscaltit as entidfiscaltit,
				c.entidfiscalsub as entidfiscalsub		
  		FROM evento.ctcontrato c
        inner join entidade.entidade ent on c.entidfiscaltit = ent.entid  			
        inner join entidade.entidade ent2 on c.entidfiscalsub = ent2.entid  		
  				WHERE
			c.ctrid = ".$_SESSION['ctrid'];


$rsDados = $db->carregar( $sql );
//$rsDados = $db->pegaLinha( $sql );

if($rsDados) extract($rsDados);

//$ctcpffiscaltit = $rsDados[0]['ctcpffiscaltit'];
//$ctcpffiscalsubst = $rsDados[0]["ctcpffiscalsubst"];
//$ctnumportfiscal = $rsDados[0]["ctnumportfiscal"];
//$ctdataportfiscal = $rsDados[0]["ctdataportfiscal"];   




//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
cabecalhoContrato( $_SESSION['ctrid'] );
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoContrato($_SESSION['ctrid']);
?> 
<script src="../includes/prototype.js"></script>
<script src="../includes/calendario.js"></script> 
<script language="JavaScript" src="/includes/funcoes.js"></script>

    <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

            <tr>
                <td class ="SubTituloDireita" align="right">Arquivo Portaria: </td>
                <td> 
                <?php
				$sql = "SELECT 
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>'
                     FROM
                        ((public.arquivo arq INNER JOIN evento.ctanexo aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.ctrid = " .$_SESSION['ctrid']."
                    AND tarq.tpaid = ".ID_PORTARIA_FISCAL."
                    LIMIT 1";
				if( $_SESSION['ctrid'] != '' ){
                	$termo = $db->pegaUm( $sql );
				}
				if( $termo ){
                	echo $termo;
                }else{
                	echo "Arquivo da Portaria não anexado";
                }
				?> 
                </td> 
            </tr>	

            <tr>
            	<td class="SubTituloDireita" align="right">Fiscal Titular: </td>
            	<td align="left">
	            	<?php

	            	$entidfiscaltit = $rsDados[0]["entidfiscaltit"];
	            	if($entidfiscaltit){
	            		
	            	  	$sql = "select 	 
	                			ent.entnome 
							from entidade.entidade ent
							left join entidade.funcaoentidade fue on ent.entid = fue.entid
							where fue.funid = 62
							and ent.entid = ".$entidfiscaltit;
	            	 	$entnomefiscaltit = $db->pegaUm($sql);
	            	 	 
	            	}
            	
            	 
            		 //$db->monta_combo('entidcontratada', $sql, 'S', "Selecione...", '', '', '', '700', 'N', 'entidcontratada',false,null,'Contratada');
            		?>
			  		<span id="entnomefiscaltit"><?=$entnomefiscaltit;?></span>
			  		<input type="hidden" name="entidfiscaltit" id="entidfiscaltit" value="<?=$entidfiscaltit;?>">
			  		<input type="button" name="pesquisar_fiscal" value="Pesquisar" style="cursor: pointer;" onclick="inserirFiscal(document.getElementById('entidfiscaltit').value);" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
            	
            	</td>           	
            </tr>
            <tr>
            	<td class="SubTituloDireita" align="right">Fiscal Substituto: </td>
            	<td align="left">
	            	<?php

	            	$entidfiscalsub = $rsDados[0]["entidfiscalsub"];
	            	if($entidfiscalsub){
	            		
	            	  	$sql = "select 	 
	                			ent.entnome 
							from entidade.entidade ent
							left join entidade.funcaoentidade fue on ent.entid = fue.entid
							where fue.funid = 62
							and ent.entid = ".$entidfiscalsub;
	            	 	$entnomefiscalsub = $db->pegaUm($sql);
	            	 	 
	            	}
            	
            	 
            		 //$db->monta_combo('entidcontratada', $sql, 'S', "Selecione...", '', '', '', '700', 'N', 'entidcontratada',false,null,'Contratada');
            		?>
			  		<span id="entnomefiscalsub"><?=$entnomefiscalsub;?></span>
			  		<input type="hidden" name="entidfiscalsub" id="entidfiscalsub" value="<?=$entidfiscalsub;?>">
			  		<input type="button" name="pesquisar_fiscal" value="Pesquisar" style="cursor: pointer;" onclick="inserirFiscalSub(document.getElementById('entidfiscalsub').value);" <?php if($somenteLeitura=="N") echo "disabled"; ?>>
            	
            	</td>           	
            </tr>

    
		<!-- tr>
			<td align='right' class="SubTituloDireita">Fiscal Titular:</td>
			<td>
				<?php /*

			
				
					// SELECT PALEATIVO QUE PEGAR TODAS AS UNIDADES INDEPENDENTE DO EXERCÍCIO
					$sql = "SELECT 
							u.usucpf as codigo, u.usunome as descricao
						FROM
							seguranca.usuario u 
						INNER JOIN 
							seguranca.usuario_sistema us on us.usucpf = u.usucpf
						WHERE
							us.sisid = ". $_SESSION['sisid'] ."
						ORDER BY 
							u.usunome";
				
					$ctcpffiscaltit = array(
											"codigo" => $ctcpffiscaltit,
											"descricao" => $ctcpffiscaltit.' - '.$usunometit
											);
					
					campo_popup('ctcpffiscaltit',$sql,'Selecione','','400x400','70', array(
																array("codigo" => "u.usucpf","descricao" => "CPF:","numeric" => "1"),
																array("codigo" => "u.usunome","descricao" => "Nome:","numeric" => "0")),
																1
																);
				
//					$db->monta_combo( "unidade", $sql, 'S', '&nbsp;', '', '', '', '' );
				?>
			</td>
		</tr>		
<tr>
			<td align='right' class="SubTituloDireita">Fiscal Substituto:</td>
			<td>
				<?php

			
				
					// SELECT PALEATIVO QUE PEGAR TODAS AS UNIDADES INDEPENDENTE DO EXERCÍCIO
					$sql = "SELECT 
							u.usucpf as codigo, u.usunome as descricao
						FROM
							seguranca.usuario u 
						INNER JOIN 
							seguranca.usuario_sistema us on us.usucpf = u.usucpf
						WHERE
							us.sisid = ". $_SESSION['sisid'] ."
						ORDER BY 
							u.usunome";
					
					$ctcpffiscalsubst = array(
											"codigo" => $ctcpffiscalsubst,
											"descricao" => $ctcpffiscalsubst.' - '.$usunomesubst
											);
																
					campo_popup('ctcpffiscalsubst',$sql,'Selecione','','400x400','70', array(
																array("codigo" => "u.usucpf","descricao" => "CPF:","numeric" => "1"),
																array("codigo" => "u.usunome","descricao" => "Nome:","numeric" => "0")),
																1
																);
					
//					$db->monta_combo( "unidade", $sql, 'S', '&nbsp;', '', '', '', '' );
				*/?>
			</td>
		</tr-->
        <tr>
                <td class ="SubTituloDireita" align="right">Número Portaria:</td>
                <td>
                	<?php
                	$ctnumportfiscal = $rsDados[0]["ctnumportfiscal"]; 
                	?>
	            	 <?= campo_texto('ctnumportfiscal', 'S', $somenteLeitura, 'Número da Portaria', 6, 10, '', '', 'left', '',  0, 'id="ctnumportfiscal" onblur="MouseBlur(this);"' ); ?>
                </td>
        </tr>
        <tr>
                <td class ="SubTituloDireita" align="right">Data Portaria:</td>
                <td>
                	<?php 
                	$ctdataportfiscal = $rsDados[0]["ctdataportfiscal"];
                	?>
    	            <?= campo_data( 'ctdataportfiscal','S', 'S', 'Data da Portaria', 'S' ); ?>
                </td>
        </tr>
        <tr>
            	<td  class ="SubTituloDireita" align="right"></td>
            	 <td>
            	   <input type="hidden" name="action" value="1" id="action">
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="enviaForm();">
            	  </td>
        </tr>
      </table>
      </form>     


<script type="text/javascript">

var d = document;

function enviaForm(){
 	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0]			= "ctnumportfiscal";		
	campos[1]			= "ctdataportfiscal";
		
					 
	tiposDeCampos[0] 	= "texto";
	tiposDeCampos[1] 	= "data";

/*	if(d.formulario.ctcpffiscaltit.value == ''){
		alert("Selecione o Fiscal Titular");
		return false;
	}

	if(d.formulario.ctcpffiscalsubst.value == ''){
		alert("Selecione o Fiscal Substituto");
		return false;
	}
*/	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
}

var caminho_atual = "evento.php";
function inserirFiscal(entidfiscaltit){
	if (entidfiscaltit){ 
			return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&busca=entnumcpfcnpj&campo=1&entid=' + entidfiscaltit,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&campo=1','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirFiscalSub(entidfiscalsub){	
	if (entidfiscalsub){ 
		return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&busca=entnumcpfcnpj&campo=2&entid=' + entidfiscalsub,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		return windowOpen( caminho_atual + '?modulo=principal/inserir_fiscal&acao=A&campo=2','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}

</script>