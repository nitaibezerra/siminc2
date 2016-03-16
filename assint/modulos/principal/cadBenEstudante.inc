<?php
$comp = new AssInt();

$paramUpdate = array();

switch ($_REQUEST['evento']){
	case 'manter':
		if ($_POST['benid']){
			$paramUpdate = array('benid' => $_POST['benid']);
		}
		
		if ($comp->manterBeneficiario($_POST, $paramUpdate )){	
			$db->sucesso($_REQUEST['modulo'], $paramUrl);	
		}else{
			redir('', 'Operação não Realizada!\nHouve Alguma Incompatibilidade nos Dados Enviados.');
		}	
	break;
	case 'excluir':
		if ($_GET['benid']){
			$dados 		 = array("benstatus" => 'I');
			$paramUpdate = array('benid' => $_GET['benid']);
			
			if ($comp->manterBeneficiario($dados, $paramUpdate )){	
				$db->sucesso($_REQUEST['modulo'], $paramUrl);	
			}else{
				redir('', 'Operação não Realizada!\nHouve Alguma Incompatibilidade nos Dados Enviados.');
			}	
		}
	break;
}

$param['where'] = array("benindbenef" => "'E'");
//if ($_REQUEST['benid']){
extract($comp->carregaBeneficiario($_REQUEST['benid'], $param));
//}

if ($benid && empty($nivid)){
	$nivid = 99999;
	$style_benjustnivel = '';		
}else{
	$style_benjustnivel = 'display:none;';		
}

// Prepara bentipo para o FORM
switch ($bentipo){
	case BEN_TIPO_EST_INS:
		${"check_bentipo" . BEN_TIPO_EST_INS} = "checked=\"checked\"";
	break;	
	case BEN_TIPO_COO_TEC:
		${"check_bentipo" . BEN_TIPO_COO_TEC} = "checked=\"checked\"";
	break;	
	default:
		${"check_bentipo" . BEN_TIPO_BRA_EXT} = "checked=\"checked\"";
	break;		
}

// Prepara bentitulacao para o FORM
switch ($bentitulacao){
	case BEN_TITU_DOISP:
		${"check_bentitulacao" . BEN_TITU_DOISP} = "checked=\"checked\"";
	break;	
	default:
		${"check_bentitulacao" . BEN_TITU_UMP} = "checked=\"checked\"";
	break;	
}


// Indica que o Beneficiário estará no contexto do programa
if ($_REQUEST['acao'] == 'A'){
	$cabecalho  = $comp->cabecalhoPrograma('', 'Programa/Projeto');
	$prgid      = $_SESSION['assint']['prgid'];
	$filtro     = array("prgid" => $prgid);
	$parametros = array(
						 '',
						 '&prgid=' . $prgid
					    );
}else{
	$filtro = array();
}

// Monta Cabeçalho
include APPRAIZ."includes/cabecalho.inc";

// Monta título da página
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Estudantes','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');
echo $cabecalho;

if($prgid) {
	$sql = "SELECT entid FROM assint.programa p WHERE prgid = $prgid";
	$entidPrograma = $db->pegaUm($sql);
}

// flag para botão salvar
$dis = false;

/*** Recupera o array com os perfis do usuário ***/
$perfis = recuperaPerfil();

/**
 * Verifica se o usuário tem acesso de gravação 
 */
if($entidPrograma) {
	/**
	 * Verifica se o usuário possui perfil de Universidade 
	 * e quais estão associadas a seu perfil
	*/
	if(in_array(PERFIL_UNIVERSIDADES, $perfis)) {
		$entids = recuperaUniversidades();
	
		if($entids) {
			if( in_array($entidPrograma, $entids) )
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

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script src="/assint/js/assint.js"></script>

<form method="POST"  name="fCadBeneficiario" onsubmit="javascript:return valida();">
<input type="hidden" name="evento" value="manter">
<input type="hidden" name="benid" value="<?=$benid?>">
<input type="hidden" name="prgid" value="<?=$prgid?>">
<input type="hidden" name="benindbenef" value="E">
<input type="hidden" name="entid" id="entid" value="<?=$entid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right'  class="SubTituloDireita">CPF:</td>
        <td>
        	<?=campo_texto('cpf','N','N','',18,14,'###.###.###-##','');?>
        	&nbsp;
			<? if (!$dis){ ?>
        	<a href="#" onclick="AbrirPopUp('?modulo=principal/popup/inserirEntidade&acao=A<?=$entid ? '&entid=' . $entid : ''; ?>', 'bEntidade', 'scrollbars=yes, width=600, heigth=500');"><img src="/imagens/gif_inclui.gif" border="0"> Adicionar Estudante por CPF</a>
			<? } ?>
        </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" width="30%">Nome:</td>
        <td>
		<?=campo_texto('bennome','S','S','',80,200,'','');?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Tipo:</td>
        <td>
			<ul style="margin: 0pt; padding: 0pt;">
				<li style="margin: 0pt; width: 160px; list-style-type: none; float: left;">
					<input id="tipo1" type="radio" <?=${"check_bentipo" . BEN_TIPO_BRA_EXT} ?> value="<?=BEN_TIPO_BRA_EXT ?>" name="bentipo"/>
					<label for="tipo1">Brasileiro no Exterior</label>
				</li>
				<li style="margin: 0pt; width: 160px; list-style-type: none; float: left;">
					<input id="tipo2" type="radio" <?=${"check_bentipo" . BEN_TIPO_EST_INS} ?> value="<?=BEN_TIPO_EST_INS ?>" name="bentipo"/>
					<label for="tipo2">Estrangeiro na Instituição</label>
				</li>				
			</ul>	
		</td>
      </tr>      
      <tr>
        <td align='right'  class="SubTituloDireita">Nº do Passaporte:</td>
        <td><?=campo_texto('benpassaporte','N','S','',30,20,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">País de Origem/Destino:</td>
        <td>
         <?
		$sql="SELECT 
				paiid as codigo, 
				paidescricao as descricao 
			  FROM 
			  	territorios.pais
			  WHERE		 
			  	paiid != " . PAIS_BRASIL . " 	
			  ORDER BY 
			  	paidescricao";
		$db->monta_combo("paiid",$sql,'S',"Selecione um País...",'','','','','S');
	 	?>
		</td>
      </tr>
        <?
		if(!$prgid)
		{
			echo "<tr><td align='right' class='SubTituloDireita'>Instituição de Origem:</td><td>";
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
			$db->monta_combo("entidorigem",$sql,'S',"Selecione uma Instituição...",'','','','','S');
			echo "</td></tr>";
		}
	 	?>
      <tr>
        <td align='right'  class="SubTituloDireita">Instituição:</td>
        <td><?=campo_texto('benuniversidade','S','S','',80,100,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Cidade:</td>
        <td><?=campo_texto('bencidade','N','S','',80,100,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Titulação:</td>
        <td>
			<ul style="margin: 0pt; padding: 0pt;">
				<li style="margin: 0pt; width: 160px; list-style-type: none; float: left;">
					<input id="titulacao1" type="radio" <?=${"check_bentitulacao" . BEN_TITU_UMP} ?> value="<?=BEN_TITU_UMP ?>" name="bentitulacao"/>
					<label for="titulacao1">Em um País</label>
				</li>
				<li style="margin: 0pt; width: 150px; list-style-type: none; float: left;">
					<input id="titulacao2" type="radio" <?=${"check_bentitulacao" . BEN_TITU_DOISP} ?> value="<?=BEN_TITU_DOISP ?>" name="bentitulacao"/>
					<label for="titulacao2">Nos Dois Países</label>
				</li>
			</ul>	
		</td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Nível de Ensino:</td>
        <td>
         <?
		$sql="(	SELECT 
					nivid as codigo, 
					nivdesc as descricao 
				FROM 
				  	assint.nivel
				WHERE		 
					nivstatus = 'A'
				ORDER BY 
				  	nivdesc
			  )UNION ALL(
			  	SELECT
			  		99999 AS codigo,
			  		'OUTROS' AS descricao
			  )";
		$db->monta_combo("nivid",$sql,'S',"Selecione um Nível...","controleNivel",'','','','S');
	 	?>
		</td>
      </tr>
      <tr id="justnivel" style="<?=$style_benjustnivel ?>">
        <td align='right'  class="SubTituloDireita">Outro Nível de Ensino:</td>
        <td><?=campo_texto('benjustnivel','S','S','',80,200,'','');?></td>
      </tr>      
      <tr>
        <td align='right'  class="SubTituloDireita">Grande Área:</td>
        <td>
         <?
		$sql="SELECT 
				garid as codigo, 
				gardesc as descricao 
			  FROM 
			  	assint.grandearea
			  WHERE		
			  	garstatus = 'A' 
			  ORDER BY 
			  	gardesc";
		$db->monta_combo("garid",$sql,'S',"Selecione uma grande área...",'','','','','S');
	 	?>
		</td>
      </tr>      
      <tr>
        <td align='right'  class="SubTituloDireita">Curso:</td>
        <td><?=campo_texto('bencurso','N','S','',80,200,'','');?></td>
      </tr>         
      <tr>
        <td align='right'  class="SubTituloDireita">Data de Início:</td>
        <td>
        	<?= campo_data2( 'bendtinicial', 'S', 'S', '', 'S' ); ?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Data Conclusão/Renovação:</td>
        <td>
        	<?= campo_data2( 'bendtfinal', 'S', 'S', '', 'S' ); ?>
        </td>
      </tr>       
	  <tr>
		<td align='right' class="SubTituloDireita">Fonte de Financiamento:</td>
		<td>
			<?php
			$sql_perfil = sprintf("select
									 fofid as codigo, 
									 fofdesc as descricao 
								   from
								     assint.fontefinanciamento
								   where
								   	fofstatus='A' 
								   order by
								    fofdesc"
							, PAIS_BRASIL);
			if ($benid){				
				$sql = sprintf("select
								 f.fofid as codigo, 
								 f.fofdesc as descricao 
							   	from
							     assint.fontefinanciamento f
							    inner join 
							     assint.beneficiariofontefin bf on bf.fofid = f.fofid
							     								   and bf.benid = '%s'
							   	where
							   	 fofstatus='A' 
							    order by
							     fofdesc"
							    , $benid);
				$nome = 'fofid';
				$$nome = $db->carregar( $sql );
			} 
			combo_popup( 'fofid', $sql_perfil, 'Selecione a(s) fonte(s) de financiamento(s)', '360x460' );
			?>
		</td>
	  </tr> 
      
	  <tr bgcolor="#CCCCCC">
	    <td>&nbsp;</td>
	    <td>
	    	<input type="submit" name="btalterar" value="Salvar" onclick="" class="botao" <?=(($dis) ? 'disabled="disabled"' : '')?>>
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btcancelar" value="Voltar" onclick="history.go(-1)" class="botao">
	    </td>
	  </tr>      
</table>
</form>
<br/>
<?
$param = array(
				"modulo" => $_GET['modulo']
			  );
$filtro['benindbenef'] = "'E'";			  
$comp->listaBeneficiario($filtro, $param, $entidPrograma);
?>
<script>

function valida(){
	var d 		= document;
	var f 		= d.fCadBeneficiario;
	var txt 	= '';
	var retorno = true;
	
	if (f.bennome.value.length < 3){
		txt += 'O campo "Nome" deve ser preenchido!';	
		retorno = false; 
	}
	
	if (f.paiid.value == ''){
		txt += '\nO campo "País de Origem/Destino" deve ser marcado!';	
		retorno = false; 
	}
	
	<? if(!$prgid) { ?>
		if (f.entidorigem.value == ''){
			txt += '\nO campo "Instituição de Origem" deve ser marcado!';	
			retorno = false; 
		}
	<? } ?>
	
	if (f.benuniversidade.value.length < 3){
		txt += '\nO campo "Instituição" deve ser preenchido!';	
		retorno = false; 
	}
	
	if (f.nivid.value == ''){
		txt += '\nO campo "Nível de Ensino" deve ser marcado!';	
		retorno = false; 
	}else if (f.nivid.value == 99999){
		if (f.benjustnivel.value.length < 3){
			txt += '\nO campo "Outro Nível de Ensino" deve ser marcado!';	
			retorno = false; 
		}
	}

	if (f.garid.value == ''){
		txt += '\nO campo "Grande Área" deve ser marcado!';	
		retorno = false; 
	}
	
	if (f.bendtinicial.value.length < 10){
		txt += '\nO campo "Data de Início" deve ser preenchido!';	
		retorno = false; 
	}		
	
	if (f.bendtfinal.value.length < 10){
		txt += '\nO campo "Data Conclusão/Renovação" deve ser preenchido!';	
		retorno = false; 
	}	
	
	if (txt != ''){
		alert(txt);
	}else{
		selectAllOptions( document.getElementById( 'fofid' ) );
	}
	
	return retorno;
}

function controleNivel(val){

	if (val == 99999){
		ctrlDisplay('justnivel', '');
	}else{
		ctrlDisplay('', 'justnivel');
	}

}
</script>