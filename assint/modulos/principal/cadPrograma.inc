<?php
$comp = new AssInt();
$comp->carregaSessionPrgid($_REQUEST['prgid']);

switch ($_REQUEST['evento']){
	case 'manter':
		if ($_POST['prgid']){
			$paramUpdate = array('prgid' => $_POST['prgid']);
			$paramUrl	 = "&prgid={$_POST['prgid']}";
		}
		$prgid = $comp->manterPrograma($_POST, $paramUpdate );
		if ($prgid){	
//			$db->sucesso("principal/cadPrograma", $paramUrl);	
			redir('?modulo=principal/cadPrograma&acao=E&prgid=' . $prgid, 'Operação Realizada com Sucesso');
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
monta_titulo('Programa/Projeto','<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

extract($comp->carregaPrograma($_REQUEST['prgid']));

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

if ($prgtipo == PROG_TIPO_MU){
	$displayB = 'none;';
	$checkM = "checked=\"checked\"";
}else{
	$displayM = 'none;';
	$checkB = "checked=\"checked\"";
}
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script src="/assint/js/assint.js"></script>

<form method="POST"  name="formulario" onsubmit="javascript: return salvar()">
<input type="hidden" name="evento" value="manter">
<input type="hidden" name="prgid" value="<?=$prgid?>">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' colspan="2" class="SubTituloCentro">Dados do Programa/Projeto</td>
      </tr>
      <tr>
        <td align='right' width="30%" class="SubTituloDireita">Nome:</td>
        <td>
		<?=campo_texto('prgnome','S','S','',80,200,'','');?>
        </td>
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
        <td align='right'  class="SubTituloDireita">Descrição:</td>
        <td>
			<?=campo_textarea('prgdescricao','S','S','',80,5,1000);?>
		</td>
      </tr>
      <tr>
        <td align='right' colspan="2" class="SubTituloCentro">Participantes</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo de Programa/Projeto</td>
		<td>
		<ul style="margin: 0pt; padding: 0pt;">
			<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;">
				<input id="bilateral" type="radio" <?=$checkB ?> value="<?=PROG_TIPO_BI; ?>" name="prgtipo" onclick="ctrlDisplay('bilateral1', ['multilateral1','multilateral2'])"/>
				<label for="bilateral">Bilateral</label>
			</li>
			<li style="margin: 0pt; width: 80px; list-style-type: none; float: left;">
				<input id="multilateral" type="radio" <?=$checkM ?> value="<?=PROG_TIPO_MU; ?>" name="prgtipo" onclick="ctrlDisplay(['multilateral1','multilateral2'], 'bilateral1')"/>
				<label for="multilateral">Multilateral</label>
			</li>
		</ul>
		</td>
	  </tr>
      <tr id="bilateral1" style="display:<?=$displayB ?>;">
        <td align='right'  class="SubTituloDireita">Informe o País:</td>
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
			if ($prgtipo == PROG_TIPO_MU){				
				$sql = sprintf("select
								 p.paiid as codigo, 
								 paidescricao as descricao 
							   from
							     territorios.pais p
							   inner join 
							   	 assint.programapais pp on pp.paiid = p.paiid
							   	 						   AND pp.prgid = %s 
							   where
							   	p.paiid != %s 
							   order by
							    paidescricao"
							    , $prgid
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
			if ($prgtipo == PROG_TIPO_MU){					
				$sql = sprintf("SELECT
								 oi.oriid AS codigo,
								 oridescricao AS descricao
							    FROM
							     entidade.organismointernacional oi 
							    INNER JOIN 
							     	assint.programaorginternacional poi ON poi.oriid = oi.oriid
							     										   AND prgid = %s								   
							    ORDER BY
							     oridescricao"
							, $prgid
				);
				$nome = 'oriid';
				$$nome = $db->carregar( $sql ); 
			}	
			combo_popup( 'oriid', $sql_perfil, 'Selecione o(s) Organismo(s) Internacional(is)', '360x460' );
			?>
		</td>
	  </tr> 
      <tr>
        <td align='right' colspan="2" class="SubTituloCentro">Dados Complementares</td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Interface:</td>
        <td>
			<?=campo_textarea('prginterface','N','S','',80,5,1000);?>
		</td>
      </tr>	 
      <tr>
        <td align='right'  class="SubTituloDireita">Data de Início:</td>
        <td>
        	<?= campo_data2( 'prgdatainicial', 'S', 'S', '', 'S' ); ?>
        </td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Data Conclusão/Renovação:</td>
        <td>
        	<?= campo_data2( 'prgdatafinal', 'S', 'S', '', 'S' ); ?>
        </td>
      </tr> 
      <tr>
        <td align='right'  class="SubTituloDireita">Meta:</td>
        <td><?=campo_texto('prgmeta','N','S','',80,200,'','');?></td>
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
			if ($prgid){				
				$sql = sprintf("SELECT
									fof.fofid as codigo, 
									fof.fofdesc as descricao  
								FROM
									assint.fontefinanciamento fof
								INNER JOIN assint.programafontefin pfo 
									ON pfo.fofid = fof.fofid
								WHERE pfo.prgid = '%s'
							   "
							    , $prgid);
								
				$nome = 'fofid';
				$$nome = $db->carregar( $sql );
			} 
			combo_popup( 'fofid', $sql_perfil, 'Selecione a(s) Fonte(s) Financeira(s)', '360x460' );
			?>
		</td>
	  </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Contato:</td>
        <td>
			<?=campo_textarea('prgcontato','N','S','',80,4,300);?>
		</td>
	  </tr>
 	  <tr bgcolor="#CCCCCC">
	    <td>&nbsp;</td>
	    <td>
	    	<input type="submit" name="btalterar" value="Salvar" class="botao" <?=(($dis) ? 'disabled="disabled"' : '')?>>
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="button" name="btcancelar" title="Ir para Lista de Programas/Projetos" value="Voltar" onclick="redireciona('?modulo=principal/listPrograma&acao=A');" class="botao">
	    	<? 
	    	if ($_REQUEST['acao'] == 'E'){
	    	?>
			&nbsp;&nbsp;&nbsp;&nbsp;	
	    	<input type="button" name="btcancelar" value="Novo Programa/Projeto" onclick="redireciona('?modulo=principal/cadPrograma&acao=A');" class="botao">	    		
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
		
	if (f.prgnome.value.length < 3){
		txt += 'O campo "Nome" deve ser preenchido!';	
		retorno = false; 
	}

	if (f.entid.value == ''){
		txt += '\nO campo "Instituição" deve ser marcado!';	
		retorno = false; 
	}

	if (f.prgdescricao.value.length < 3){
		txt += '\nO campo "Descrição" deve ser preenchido!';	
		retorno = false; 
	}

	if (f.prgdatainicial.value.length < 10){
		txt += '\nO campo "Data de Início" deve ser preenchido!';	
		retorno = false; 
	}	

	if (f.prgdatafinal.value.length < 10){
		txt += '\nO campo "Data Conclusão/Renovação" deve ser preenchido!';	
		retorno = false; 
	}	
	
	if (txt != ''){
		alert(txt);
	}else{
		selectAllOptions( document.getElementById( 'arrPaiid' ) );
		selectAllOptions( document.getElementById( 'oriid' ) );
		selectAllOptions( document.getElementById( 'fofid' ) );
	}
	
	return retorno;		
}
</script>