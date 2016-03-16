<?php
$comp = new AssInt();
$comp->carregaSessionAcoid($_REQUEST['acoid']);

switch ($_REQUEST['evento']){
	case 'manter':
		if ($_POST['acoid']){
			$paramUpdate = array('acoid' => $_POST['acoid']);
			$paramUrl	 = "&acoid={$_POST['acoid']}";
		}
		$acoid = $comp->manterAcordo($_POST, $paramUpdate );
		if ($acoid){
			redir('?modulo=principal/cadAcordo&acao=E&acoid=' . $acoid, 'Operação Realizada com Sucesso');
		}else{
			redir('', 'Operação não Realizada!\nHouve Alguma Incompatibilidade nos Dados Enviados.');
		}			
	break;
	default:
	break;
}	

// Monta Cabeçalho
include APPRAIZ."includes/cabecalho.inc";

// Monta título da página
echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Acordo','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

extract($comp->carregaAcordo($_REQUEST['acoid']));

// flag para botão salvar
$dis = false;

/*** Recupera o array com os perfis do usuário ***/
$perfis = recuperaPerfil();

/**
 * Verifica se o usuário tem acesso de gravação 
 */
if($entid) {
	/**
	 * Verifica se o usuário possui perfil de Universidade 
	 * e quais estão associadas a seu perfil
	*/
	if(in_array(PERFIL_UNIVERSIDADES, $perfis)) {
		$entids = recuperaUniversidades();
	
		if($entids) {
			if( in_array($entid, $entids) )
				$dis = false;
			else
				$dis = true;
		}
	} else {
		$dis = true;
	}
}else{
	if(in_array(PERFIL_CONSULTA, $perfis)) {
		$dis = true;
	}
}

if ($acotipo == PROG_TIPO_MU){
	$displayB = 'none;';
	$checkM = "checked=\"checked\"";
}else{
	$displayM = 'none;';
	$checkB = "checked=\"checked\"";
}
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="/assint/js/assint.js"></script>

<form method="POST"  name="formulario" onsubmit="javascript: return salvar()">
	<input type="hidden" name="evento" value="manter">
	<input type="hidden" name="acoid" value="<?=$acoid?>">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' colspan="2" class="SubTituloCentro">Dados do Acordo</td>
		</tr>
		<tr>
			<td align='right' width="30%" class="SubTituloDireita">Título do Convênio:</td>
			<td><?=campo_texto('acoconvenio','S','S','',80,200,'','');?></td>
		</tr>
		<tr>
        <td align='right' class="SubTituloDireita">Instituição:</td>
        <td>
         <?
         $where = "";
         if( !$dis ) 
         {
	         /**
			  * Verifica se o usuário possui perfil de Universidade 
			  * e quais estão associadas a seu perfil
			  */
			 if( in_array(PERFIL_UNIVERSIDADES, $perfis) && !in_array(PERFIL_SUPER_USUARIO, $perfis) )
			 {
			 	$entids = recuperaUniversidades();
			 	
			 	$where = ($entids) ? ' WHERE e.entid in ('.implode(",", $entids).') ' : ' WHERE e.entid not in (select entid from assint.entidadeassessoriainternacional where entstatus = \'A\')';
			 }
         }
         
		$sql="SELECT 
				e.entid as codigo, 
				e.entnome as descricao 
			  FROM 
			  	entidade.entidade e
			  INNER JOIN 
			  	assint.entidadeassessoriainternacional ea ON ea.entid = e.entid
			  												 AND ea.entstatus = 'A'
			  ".$where."
			  ORDER BY 
			  	e.entnome";
		$db->monta_combo("entid",$sql,'S',"Selecione uma Instituição...",'','','','','S');
	 	?>
        </td>
      </tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome do Representante da Instituição:</td>
	        <td><?=campo_texto('acorepresentante','N','S','',80,200,'','');?></td>
		</tr>
		<tr>
			<td align='right'  class="SubTituloDireita">Cargos:</td>
			<td>
				<?php
				$sql="SELECT 
						c.carid as codigo, 
						c.cardesc as descricao 
					  FROM 
					  	assint.cargo c
					  WHERE c.carstatus = 'A'
					  ORDER BY
					  	c.cardesc";
				
				$db->monta_combo("carid",$sql,'S',"Selecione um Cargo...",'','','','','S');
			 	?>
			</td>
		</tr>
		<tr>
			<td align='right' colspan="2" class="SubTituloCentro">Participantes</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Tipo do Acordo</td>
			<td>
				<ul style="margin: 0pt; padding: 0pt;">
					<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;">
						<input id="bilateral" type="radio" <?=$checkB ?> value="<?=PROG_TIPO_BI; ?>" name="acotipo" onclick="ctrlDisplay('bilateral1', ['multilateral1','multilateral2'])"/>
						<label for="bilateral">Bilateral</label>
					</li>
					<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;">
						<input id="multilateral" type="radio" <?=$checkM ?> value="<?=PROG_TIPO_MU; ?>" name="acotipo" onclick="ctrlDisplay(['multilateral1','multilateral2'], 'bilateral1')"/>
						<label for="multilateral">Multilateral</label>
					</li>
				</ul>
			</td>
		</tr>
		<tr id="bilateral1" style="display:<?=$displayB ?>;">
			<td align='right'  class="SubTituloDireita">Informe o País:</td>
	        <td>
				<?php
				$sql="SELECT 
						paiid as codigo, 
						paidescricao as descricao 
					  FROM 
					  	territorios.pais
					  WHERE		 
					  	paiid != " . PAIS_BRASIL . " 	
					  ORDER BY 
					  	paidescricao";
				$db->monta_combo("paiid",$sql,'S',"Selecione um País...",'','');
			 	?>
			</td>
		</tr>    
		<tr id="multilateral1" style="display:<?=$displayM ?>;">
			<td align='right' class="SubTituloDireita">Informe o(s) País(es):</td>
			<td>
				<?php
				$sql_perfil = sprintf("select
										 paiid as codigo, 
										 paidescricao as descricao 
									   from
									     territorios.pais
									   where
									   	paiid != %s 
									   order by
									    paidescricao"
								, PAIS_BRASIL);
				if ($acotipo == PROG_TIPO_MU){				
					$sql = sprintf("select
									 p.paiid as codigo, 
									 paidescricao as descricao 
								   from
								     territorios.pais p
								   inner join 
								   	 assint.acordopais ac on ac.paiid = p.paiid
								   	 						   AND ac.acoid = %s 
								   where
								   	p.paiid != %s 
								   order by
								    paidescricao"
								    , $acoid
									, PAIS_BRASIL);
									
					$nome = 'arrPaiid';
					$$nome = $db->carregar( $sql );
				} 
				combo_popup( 'arrPaiid', $sql_perfil, 'Selecione o(s) País(es)', '360x460' );
				?>
			</td>
		</tr> 
		<tr id="multilateral2" style="display:<?=$displayM ?>;">
			<td align='right' class="SubTituloDireita">Informe o(s) Organismo(s) Internacional(is):</td>
			<td>
				<?php
				$sql_perfil = sprintf("SELECT
										oriid AS codigo,
										oridescricao AS descricao
									   FROM
									    entidade.organismointernacional								   
									   ORDER BY
									   	oridescricao");
				if ($acotipo == PROG_TIPO_MU){					
					$sql = sprintf("SELECT
									 oi.oriid AS codigo,
									 oridescricao AS descricao
								    FROM
								     entidade.organismointernacional oi 
								    INNER JOIN 
								     	assint.acordoorginternacional poi ON poi.oriid = oi.oriid
								     										   AND acoid = %s								   
								    ORDER BY
								     oridescricao"
								, $acoid
					);
					$nome = 'oriid';
					$$nome = $db->carregar( $sql ); 
				}	
				combo_popup( 'oriid', $sql_perfil, 'Selecione o(s) Organismo(s) Internacional(is)', '360x460' );
				?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloDireita">
			<center><strong>Instituições Parceiras</strong></center>
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<table id="tabelaParceiras" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="100%" >
					<thead>
						<tr align="center">
							<td>&nbsp;</td>	
							<td>Instituição Parceira</td>
							<td>Nome da Instituição</td>
						</tr>
					</thead>
					<?php
					if($_REQUEST['acoid']){
						$sql = "SELECT * FROM assint.acordoinstituicao WHERE acoid = '".$_REQUEST['acoid']."'";
						$insParc = $db->carregar($sql);
						
						$sql = "SELECT * FROM assint.instituicao ORDER BY insdesc ASC";
						$comboParceiras = $db->carregar($sql);
						
						//ver($insParc, 1);
						//if(count($insParc) > 0){
						if($insParc != null){							
							foreach($insParc as $parceiras){
								//echo $parceiras['insid']."<br>";
								//echo $parceiras['acidescricao']."<br>";
								$botaoExcluir = ($dis) ? "<img src=\"/imagens/excluir_01.gif\" alt=\"Excluir\" title=\"Excluir\" disabled=\"disabled\">" : "<img src=\"/imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\">";
								
								$html =		"
												<tr>
													<td>".$botaoExcluir."</td>
													<td>
													<select id=\"insid[]\" name=\"insid[]\">
														<option>Selecione...</option>												
											";								
								
								foreach ($comboParceiras as $cParceria){
									
									$selectCombo = ($cParceria['insid'] == $parceiras['insid']) ? 'selected=\"selected\"' : '';
									
									$html .= "<option {$selectCombo} value=\"{$cParceria['insid']}\">".$cParceria['insdesc']."</option>";
									
								}								
											
								$html .= 	"
												</select>
													</td>
													<td><input type=\"text\" id=\"acidescricao[]\" name=\"acidescricao[]\" class=\"normal\" size=\"40\"value=\"{$parceiras['acidescricao']}\"></td>
												</tr>
											";
								//<input type=\"text\ id=\"acidescricao[]\" name=\"acidescricao[]\" class=\"normal\" size=\"40\" value=\"{$parceiras['acidescricao']}\">
								echo $html;
							}
						}
					}
					?>
				</table>
				<input type="button" value="Adicionar Linha" onclick="addLinhaParceiras()" <?=(($dis) ? 'disabled="disabled"' : '')?> />
			</td>
		</tr>
		<!-- 
		<tr>
			<td colspan="2">			
				<hr style="color: blue; padding: 0px; margin: 0px;">
				<a onclick="adcionarParcerias()" style="cursor: pointer;" >Adicionar Instituições Parceiras</a>			
			</td>
		</tr>
		-->
		<tr>
			<td align='right' class="SubTituloDireita">Atividade(s):</td>
			<td>
				<?php
				$sql_perfil = sprintf("SELECT
										atiid as codigo, 
										atidesc as descricao 
									   FROM assint.atividade 
									   WHERE atistatus = 'A'
									   ORDER BY atidesc ASC");
				if ($_REQUEST['acoid']){				
					$sql = sprintf("SELECT
										ati.atiid as codigo, 
										ati.atidesc as descricao  
									FROM
										assint.atividade ati
									INNER JOIN assint.acordoatividade aat 
										ON aat.atiid = ati.atiid
									WHERE aat.acoid = '".$_REQUEST['acoid']."'
								   "
								    , $acoid);
									
					$nome = 'atiid';
					$$nome = $db->carregar( $sql );
				} 
				combo_popup( 'atiid', $sql_perfil, 'Atividade(s)', '360x460' );
				?>
			</td>
		</tr> 
		<tr>
			<td align='right' class="SubTituloDireita">Fonte(s) de Financiamento(s):</td>
			<td>
				<?php
				$sql_perfil = sprintf("SELECT
										fofid as codigo, 
										fofdesc as descricao 
									   FROM assint.fontefinanciamento 
									   WHERE fofstatus = 'A'
									   ORDER BY fofdesc ASC");
				if ($_REQUEST['acoid']){				
					$sql = sprintf("SELECT
										fof.fofid as codigo, 
										fof.fofdesc as descricao  
									FROM
										assint.fontefinanciamento fof
									INNER JOIN assint.acordofontefin aco 
										ON aco.fofid = fof.fofid
									WHERE aco.acoid = '".$_REQUEST['acoid']."'
								   "
								    , $acoid);
									
					$nome = 'fofid';
					$$nome = $db->carregar( $sql );
				} 
				combo_popup( 'fofid', $sql_perfil, 'Selecione a(s) Fonte(s) Financeira(s)', '360x460' );
				?>
			</td>
		</tr> 
		<tr>
			<td align='right' colspan="2" class="SubTituloCentro">Dados Complementares</td>
		</tr>      	 
		<tr>
			<td align='right'  class="SubTituloDireita">Data de Início:</td>
			<td><?= campo_data2( 'acodatainicial', 'S', 'S', '', 'S' ); ?></td>
		</tr>
		<tr>
			<td align='right'  class="SubTituloDireita">Data de Validade:</td>
	        <td><?= campo_data2( 'acodatafinal', 'S', 'S', '', 'S' ); ?></td>
		</tr>
		<tr>
			<td align='right'  class="SubTituloDireita">Contato:</td>
	        <td><?=campo_textarea('acocontato','N','S','',80,4,500);?></td>
		</tr>
		<tr>
			<td align='right'  class="SubTituloDireita">Status Atual:</td>
	        <td>
		        <select name="acosituacao">
		        	<option value="">Selecione um status...</option>
		        	<option <?=($acosituacao == 'A') ? 'selected="selected"' : '' ?> value="A">Vigente (Atividades em Curso)</option>
		        	<option <?=($acosituacao == 'S') ? 'selected="selected"' : '' ?> value="S">Vigente (Sem Atividades)</option>
		        	<option <?=($acosituacao == 'E') ? 'selected="selected"' : '' ?> value="E">Em Extinção </option>
		        </select>
		        <!-- <img border="0" src="imagens/obrig.gif" title="Indica campo obrigatório." alt="Indica campo obrigatório." /> -->
		        <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." alt="Indica campo obrigatório." />		        
	        </td>
		</tr>
		<tr bgcolor="#CCCCCC">
			<td>&nbsp;</td>
			<td>		    	
		    	<input type="button" name="btgravar" value="Salvar" class="botao" onClick="salvar()" <?=(($dis) ? 'disabled="disabled"' : '')?>>
		    	&nbsp;&nbsp;&nbsp;&nbsp;
		    	<input type="button" name="btcancelar" title="Ir para Lista de Acordos" value="Voltar" onclick="redireciona('?modulo=principal/listAcordo&acao=A');" class="botao">
		    	<? 
		    	if ($_REQUEST['acoid']){
		    	?>
				&nbsp;&nbsp;&nbsp;&nbsp;	
		    	<input type="button" name="btcancelar" value="Novo Acordo" onclick="redireciona('?modulo=principal/cadAcordo&acao=E');" class="botao">	    		
		    	<?	
		    	}
		    	?>
			</td>
		</tr>      
	</table>
</form>
<script type="text/javascript">
function salvar(){
	var d 		= document;
	var f 		= d.formulario;
	var txt 	= '';
	var retorno = true;
		
	if (f.acoconvenio.value.length < 3){
		txt += 'O campo "Título do Convênio" deve ser preenchido!';
		f.acoconvenio.focus();	
		retorno = false; 
	}
	
	if (f.entid.value == '' || f.carid.value == 'Selecione um Cargo...'){
		txt += '\nO campo "Instituição" deve ser preenchido!';
		f.entid.focus();	
		retorno = false; 
	}	
	
	if (f.carid.value == '' || f.carid.value == 'Selecione um Cargo...'){
		txt += '\nO campo "Cargo" deve ser preenchido!';
		f.carid.focus();	
		retorno = false; 
	}	

	if (f.acodatainicial.value.length < 10){
		txt += '\nO campo "Data de Início" deve ser preenchido!';
		f.acodatainicial.focus();	
		retorno = false; 
	}	

	if (f.acodatafinal.value.length < 10){
		txt += '\nO campo "Data de Validação" deve ser preenchido!';
		f.acodatafinal.focus();	
		retorno = false; 
	}
	
	if (f.acosituacao.value == '' || f.acosituacao.value == 'Selecione um status...'){
		txt += '\nO campo "Status Atual" deve ser preenchido!';
		f.acosituacao.focus();	
		retorno = false; 
	}
	
	if (txt != ''){
		alert(txt);
	}else{
	
		selectAllOptions( document.getElementById( 'arrPaiid' ) );
		selectAllOptions( document.getElementById( 'oriid' ) );
		selectAllOptions( document.getElementById( 'fofid' ) );
		selectAllOptions( document.getElementById( 'atiid' ) );
		
		var insid = document.getElementById( 'insid[]' );
				
		if (insid){			
			//selectAllOptions( document.getElementById( 'insid[]' ) );
			//selectAllOptions( document.getElementById( 'acidescricao[]' ) );
		}
		
		f.submit();
	}		
	
	return retorno;		
}

function adcionarParcerias(){
	var janela = window.open( "assint.php?modulo=principal/popup/cadastraInstituicaoParceira&acao=E", 'blank', 'height=500,width=600,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes' );
	janela.focus();
}

function addLinhaParceiras(){

	var tabela 		= document.getElementById("tabelaParceiras");
	var nrlinha 	= tabela.rows.length;	
	var nvlinha		= tabela.insertRow(tabela.rows.length);
	nvlinha.style.backgroundColor = '#f5f5f5';
	
	//alert(tabela+" - "+qtdlinhas+" - "+nvlinha);
	
	//IMAGEM QUE IDENTIFICA CAMPO OBRIGATÓRIO
	var imgObrigatorio = document.createElement( "img" );
	imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
	imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );	
	
	//B0TÃO EXCLUIR 
	var img = document.createElement( "img" );
	img.setAttribute( "src", "/imagens/excluir.gif" );
	img.setAttribute( "title", "Excluir" );
	img.setAttribute( "alt", "Excluir" );
	img.setAttribute( "style", "cursor: pointer;");
	img.setAttribute('onclick', 'removeLinhaParceiras( this.parentNode.parentNode.rowIndex )');
	
	//COMBO INSTITUIÇÃO PARCEIRA
	var insParceira = document.createElement('select');
	insParceira.setAttribute('name', 'insid[]');
	insParceira.setAttribute('id', 'insid[]');
	
	var optionParceira = document.createElement('option');	
	optionParceira.setAttribute('value', '');
	optionParceira.innerHTML = 'Selecione...';	
	insParceira.appendChild( optionParceira ); 
	
	<?php
	$sql = "SELECT * FROM assint.instituicao ORDER BY insdesc ASC";
	$insParceiras = $db->carregar($sql);	
	$total = count($insParceiras);
	
	for($x=0;$x<$total;$x++){		
	?>
	
	var option = document.createElement( 'option' );
	option.setAttribute( 'value', '<?=$insParceiras[$x]['insid'];?>' );
	option.innerHTML = '<?=$insParceiras[$x]['insdesc'];?>';
	insParceira.appendChild( option );
	
	<? } ?>	
	
	//CAMPO NOME DA INSTITUIÇÃO 
	var nomeParceira = document.createElement( "input" );
	nomeParceira.setAttribute( "type", "text" );
	nomeParceira.setAttribute( "name", "acidescricao[]" );
	nomeParceira.setAttribute( "id", "acidescricao[]" );	
	nomeParceira.setAttribute( "size", "40" );
	nomeParceira.className = "normal";
	
	celula = nvlinha.insertCell( nvlinha.cells.length );	
	celula.appendChild( img );
	
	celula = nvlinha.insertCell( nvlinha.cells.length );	
	celula.appendChild( insParceira );
	celula.appendChild( imgObrigatorio );	
	
	celula = nvlinha.insertCell( nvlinha.cells.length );	
	celula.appendChild( nomeParceira );
	celula.appendChild( imgObrigatorio );

}
function removeLinhaParceiras(nrlinha){	
	
	var tabela = document.getElementById( "tabelaParceiras" );			
			
	if(confirm("Deseja excluir o registro? n: "+nrlinha))
	{				
		tabela.deleteRow(nrlinha);
	}
}
</script>