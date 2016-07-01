<?
 /*
   Sistema Simec
   Setor responsvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com), Fabrício Mendonça (fabriciomendonca@gmail.com)
   Mdulo:licaoaprendida.inc
   Finalidade: administração de lições aprendidas de acordo com o projeto especial
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;

if ($_REQUEST['pjeid']) $_SESSION['pjeid']=$_REQUEST['pjeid'];
else $_REQUEST['pjeid']=$_SESSION['pjeid'];

if (! $_SESSION['pjeid']) 
{
	?>
	<script>alert('Escolha primeiro o Projeto Especial!');
	history.back();</script>
	<?
	$db->close();
	exit();
}



$lapstatus = $aprovar_check;
$acerto = $acerto_check;

/**
 * Insere uma linha na tabela licaoaprendida
 * 
 * @param integer pjeid
 * @param string usucpf
 * @param string acerto
 * @param string descricao
 * @return integer
 */

function buscanome($cpf)
{
	global $db;
	$sql = "select usunome from seguranca.usuario where usucpf = '".$cpf."'";
	$nome = $db->pegaum($sql);
	return $nome;
}


function inserir_licao( $pjeid, $titulo,$usucpf, $acerto, $lapdescricao, $acpid, $prpid, $lapstatus )
{
	global $db;
	$sql = "select nextval('monitora.licaoaprendida_lapid_seq'::regclass)";
	$id = $db->pegaUm( $sql );
	
	$acerto = $acerto == '1' ? 'true' : 'false';

     if ($lapstatus == ""){
		$sql = "insert into monitora.licaoaprendida(lapid, lapdsc,lapdescricao, tpasnacerto, usucpf, pjeid, acpid, prpid, lapstatus)"
		 . " values("
		 . $id.", "
		 . "'".$titulo."', "
		 . "'".$lapdescricao."', "
		 . $acerto.", "
		 . "'".$usucpf."', "
		 . $pjeid.", "
		 . $acpid.", "
		 . $prpid.", "
		 . "'P'"
		 . ")";
	}else 
	{
	$sql = "insert into monitora.licaoaprendida(lapid, lapdsc,lapdescricao, tpasnacerto, usucpf, pjeid, acpid, prpid, lapstatus)"
		 . " values("
		 . $id.", "
		 . "'".$titulo."', "
		 . "'".$lapdescricao."', "
		 . $acerto.", "
		 . "'".$usucpf."', "
		 . $pjeid.", "
		 . $acpid.", "
		 . $prpid.", "
		 . "'".$lapstatus."'"
		 . ")";
	}
		 
	if ( !$db->executar( $sql ) )
	{
		return 0;
	}
	return $id;
}

/**
 * Atualiza a lição aprendida de acordo com o id
 *
 * @param integer $lapid
 * @param string $titulo
 * @param string $acerto
 * @param string $descricao
 * @return integer
 */

function atualizar_licao( $lapid, $titulo, $acpid, $prpid, $acerto, $lapdescricao, $lapstatus )
{
	global $db;
	
	$acerto = $acerto == '1' ? 'true' : 'false';
	
	if ($lapstatus == "")
	{
	$set = "tpasnacerto=".$acerto.", "
		 . "lapdescricao='".$lapdescricao."', "
		 . "lapdsc='".$titulo."', "
		 . "acpid=".$acpid.", "
		 . "prpid=".$prpid.", "
		 . "lapstatus = 'P'";
	}
	else 
	{
		$set = "tpasnacerto=".$acerto.", "
		 . "lapdescricao='".$lapdescricao."', "
		 . "lapdsc='".$titulo."', "
		 . "acpid=".$acpid.", "
		 . "prpid=".$prpid.", "
		 . "lapstatus = '".$lapstatus."'";
	}
	
	$sql = "update monitora.licaoaprendida set ".$set." where lapid=".$lapid;
	
	
	if ( !$db->executar( $sql ) )
	{
		return 0;
	}
	return 1;
}


function inserir_areas( $lapid, $interesse, $influencia, $editar = 0 )
{
	global $db;
	if( $editar )
	{
		$sql = "delete from monitora.licaoaprendida_areainteresse where lapid=".$lapid;
		$db->executar( $sql );
		$sql = "delete from monitora.licaoaprendida_areainfluencia where lapid=".$lapid;
		$db->executar( $sql );
	}
	foreach( $interesse as $tpaid )
	{
		$sql = "insert into monitora.licaoaprendida_areainteresse(tpaid, lapid)"
			 . "values("
			 . $tpaid.", "
			 . $lapid
			 . ")";
		$db->executar( $sql );
	}
	foreach( $influencia as $tpaid )
	{
		$sql = "insert into monitora.licaoaprendida_areainfluencia(tpaid, lapid)"
			 . "values("
			 . $tpaid.", "
			 . $lapid
			 . ")";
			 
		$db->executar( $sql );
	}
	
}

function excluir_licao( $lapid )
{

	global $db;
	$sql = "delete from monitora.licaoaprendida_areainteresse where lapid=".$lapid;
	$db->executar( $sql );
	$sql = "delete from monitora.licaoaprendida_areainfluencia where lapid=".$lapid;
	$db->executar( $sql );
	$sql = "delete from monitora.licaoaprendida where lapid=".$lapid;
	$db->executar( $sql );

}

//Verifica se o formulário foi submetido
if( $act2 == "submit" )
{
	if( $lapid )
	{		
		if( atualizar_licao( $lapid, $titulo, $acpid, $prpid, $acerto, $lapdescricao, $lapstatus ) )
		{
			inserir_areas( $lapid, $interesse, $influencia, 1 );
		}
	}
	else
	{
		if( $lapid = inserir_licao( $pjeid,$titulo,$_SESSION[ "usucpf" ],$acerto,$lapdescricao, $acpid, $prpid, $lapstatus ) )
		{
			inserir_areas( $lapid, $interesse, $influencia );
		}
	}
	$db->commit();
	?>
		<script>
		alert("Informações salvas com sucesso.");
		document.location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/licaoaprendida&acao=A&pjeid=<?=$pjeid?>";
		</script>
	<?
	
}
else if( $act2 == "excluir" )
{
	excluir_licao( $lapid );
	$db->commit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>

<br>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<?
$db->cria_aba($abacod_tela,$url, '');

switch ( $_REQUEST[ "act" ] )
{
	case "inserir":
		$titulo_modulo = "Inclusão de Lição Aprendida";
		break;
	case "atualizar":
		$titulo_modulo = "Administração de Lição Aprendida";
		break;
}

monta_titulo($titulo_modulo, '');

//Query para montar o cabeçalho da página com as informações do projeto
$sql="select pe.pjesnvisivel as visivel,pe.pjesndatafechada as projfechado,pe.pjecod, pe.pjedsc,to_char(pe.pjedataini,'dd/mm/YYYY') as pjedataini,to_char(pe.pjedatafim,'dd/mm/YYYY') as pjedatafim,p.prodsc,pe.pjeprevistoano,pe.pjevlrano from monitora.projetoespecial pe inner join produto p on p.procod=pe.procod where pjeid=".$_REQUEST['pjeid'];
$res=$db->pegalinha($sql);
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$pjeinimt=0;
$pjefimmt=0;

$sql = "select usucpf from usuarioresponsabilidade where pjeid = ".$_REQUEST['pjeid']." and pflcod = 47";
$usucpfcoord = $db->pegaum($sql);

?>
<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"  align="center" style="color:#808080;">
  <tr>
    <td align='right' class="SubTituloDireita">Projeto :</td>
    <td><b><?=$pjecod?>&nbsp;-&nbsp;<?=$pjedsc?></b></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Período do Projeto:</td>
    <td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
  </tr>  
  <? if ($pjefimmt or $pjeinimt)
  {?>
      <tr>
    <td align='right' class="SubTituloDireita">Período da Macro-Etapa Agregadora :</td>
    <td><b><?=$pjeinimt?>&nbsp; - &nbsp;<?=$pjefimmt?></b></td>
  </tr>
  <?}?>
  <tr>
    <td align='right' class="SubTituloDireita">Produto :</td>
    <td><b><?=$prodsc?></b></td>
  </tr>
        <tr>
    <td align='right' class="SubTituloDireita">Físico e Financeiro :</td>
    <td><b><?=$pjeprevistoano?>&nbsp;/&nbsp;<?='R$ '.number_format($pjevlrano,2,',','.')?></b></td>
  </tr> 
    <tr>
  <td></td>
  <td>
  <input type="button" name="btequipe" value="Equipe do Projeto" onclick="mostra_equipe()">
  </td>
  </tr>

<? // Formulário de inclusão
	if( $_REQUEST[ "act" ] == "inserir" || $_REQUEST[ "act2" ] == "atualizar" || $_REQUEST[ "act2" ] == "submit" )
	{
		
		if( $_REQUEST[ "act2" ] == "atualizar" )
		{
			
			
			$sql = "select *  from monitora.licaoaprendida where lapid=".$lapid;
			$linha = $db->pegaLinha( $sql );
			$acerto = $linha[ "tpasnacerto" ] == 't' ? '1' : '0';
			$lapdescricao = $linha[ "lapdescricao" ];
			$titulo = $linha[ "lapdsc" ];
			$criador = $linha["usucpf"];
			$lapstatus = $linha["lapstatus"];
			$acpid = $linha["acpid"];
			$prpid = $linha["prpid"];
			
		}
		
		if ($_REQUEST[ "act" ] == "inserir")
		{
			$criador = $_SESSION['usucpf'];
		}
		
		
		//Define quem pode editar ou manipular determinada lição somente o dono ou coordenador ou no metodo de inserção de uma nova lição
		if (($usucpfcoord == $_SESSION['usucpf']) or (($criador == $_SESSION['usucpf']) and $lapstatus == 'P') or ($_REQUEST[ "act" ] == "inserir"))
		{
			$habil = 'S';
		}
		else 
		{
			$habil = 'N';
		}
		
			
		$sql_tipoarea = "select tpaid as codigo, tpadsc as descricao from public.tipoarea where tpastatus='A' order by tpadsc asc";
?>
<form name="frmIncluirLicao" method="post">
<input type="hidden" name="pjeid" id="pjeid" value="<?=$_REQUEST[ "pjeid" ]?>" />
<input type="hidden" name="act2" id="act2" value="submit" />
<? if( $lapid ) { ?>
<input type="hidden" name="lapid" id="lapid" value="<?=$lapid?>" />
<? } 
?>
	<br>
	<tr>
	    <td align='right' class="SubTituloDireita">Título:</td>
		<td>
		<?=campo_texto('titulo','S',$habil,'',77,80,'','','','Entre com o título da lição aprendida.');?>
		</td>
	</tr>
	<tr>
    	<td align='right' class="SubTituloDireita">Proposto por:</td>
    	<td><b><?=buscanome($criador);?></b></td>
  	</tr>
	<tr>
	    <td align='right' class="SubTituloDireita">Lição propriamnete dita:</td>
		<td>
		<?=campo_textarea('lapdescricao','S',$habil,'',80,3,'','','','Entre com a descrição da lição propriamente dita.');?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita">Área de conhecimento:</td>
		<td>
			<? $sql = "select acpid as codigo, acpdsc as descricao from public.areaconhecimentoprojeto where acpstatus='A'"; ?>
			<? $db->monta_combo( "acpid", $sql,$habil, "Selecione a área de conhecimento", '', '', "Selecione uma área de conhecimento.", 400 );?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita">Processo:</td>
		<td>
			<? $sql = "select prpid as codigo, prpdsc as descricao from public.processoprojeto where prpstatus='A'"; ?>
			<? $db->monta_combo( "prpid", $sql, $habil, "Selecione o processo", '', '', "Selecione um processo", 400 );?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Áreas de interesse:</td>
		<td>
			<? $sql_auxiliar = "select t.tpaid as codigo, t.tpadsc as descricao from public.tipoarea t left join monitora.licaoaprendida_areainteresse l on t.tpaid = l.tpaid where t.tpastatus='A' and l.lapid =".$lapid." order by t.tpadsc asc" ?>
			<? $interesse = $lapid ? $db->carregar( $sql_auxiliar ) : ''; ?>
			<? combo_popup( 'interesse', $sql_tipoarea, 'Selecione as áreas de interesse', '400x200') ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Áreas de influência:</td>
		<td>
			<? $sql_auxiliar = "select t.tpaid as codigo, t.tpadsc as descricao from public.tipoarea t left join monitora.licaoaprendida_areainfluencia l on t.tpaid = l.tpaid where t.tpastatus='A' and l.lapid =".$lapid." order by t.tpadsc asc" ?>
			<? $influencia = $lapid ? $db->carregar( $sql_auxiliar ) : ''; ?>
			<? combo_popup( 'influencia', $sql_tipoarea, 'Selecione as áreas de incluência', '400x200') ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Foi um acerto?</td>
		<td>
			<input type="radio" <?= $habil =='S'? '':'disabled'?> name="acerto_check" value="1" id="acerto_1" <?= $acerto == '1' ? 'checked="checked"' : '' ?> />
			<label for="acerto_1">Sim</label>
			<input type="radio" <?= $habil =='S'? '':'disabled'?> name="acerto_check" value="0" id="acerto_0" <?= $acerto == '0' ? 'checked="checked"' : '' ?> />
			<label for="acerto_0">Não</label>
			<input type="hidden" name="acerto" />
		</td>
	</tr>
	
	<? if($usucpfcoord == $_SESSION['usucpf'])
	{?>
	<tr>
		<td align='right' class="SubTituloDireita">Esta lição é valida?</td>
		<td>
			<input type="radio" <?= $habil =='S'? '':'disabled'?> name="aprovar_check" value="A" id="aprovar_1" <?= $lapstatus == 'A' ? 'checked="checked"' : '' ?> />
			<label for="acerto_1">Sim</label>
			<input type="radio" <?= $habil =='S'? '':'disabled'?> name="aprovar_check" value="P" id="aprovar_0" <?= $lapstatus == 'P' ? 'checked="checked"' : '' ?> />
			<label for="acerto_0">Não</label>
			<input type="hidden" name="acerto" />
		</td>
	</tr>
	
	<?}
	//esconde o botão salvar quando não tem permisão de acesso
	if ($habil == 'S')
	{?>
	<tr bgcolor="#cccccc">
		<td>&nbsp;</td>
		<td>
			<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="validar_cadastro();">
		</td>
	</tr>
	<?}?>
</form>
<?
	}//Fim do formulário de inclusão
?>
</table>
<?
//Mostra as lições aprendidas de acordo com o id do projeto especial
$pjeid = $_REQUEST[ "pjeid" ];




$sql = "select l.usucpf as criador, l.tpasnacerto, l.lapstatus, l.lapid, l.lapdescricao as lapdescricao from monitora.licaoaprendida l where l.pjeid=".$pjeid." ORDER BY lapdescricao"  ;
$rs = $db->carregar( $sql );

?>
<br>
<form name="formulario" method="POST">
	<input type="hidden" name="lapid" />
	<input type="hidden" name="act2" />
</form>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  	<td class="title"><strong>Ações</strong></td>
		<td class="title"><strong>Lições aprendidas</strong></td>
		<td class="title"><strong>Experiência</strong></td>
	</tr>
	<?
	$quant_licoes = 0;
	$i=0;	
	if( $rs ){
	foreach ( $rs as $linha )
		{
			foreach($linha as $k=>$v) ${$k}=$v;
			if (($lapstatus == 'A') or ($usucpfcoord == $_SESSION['usucpf']) or ($criador == $_SESSION['usucpf']))
			{
				//if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
				
	?>
	<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
	<td style="width:65px; text-align:left; padding:3px;">
	<? 
	//permisão para visualizar lição (todos perfis permitem)
	if(1)
	{
	$quant_licoes++;	
	?>
	<img border="0" src="../imagens/alterar.gif" title="Alterar a lição." onclick="altera_licao('<?=$lapid?>')">&nbsp;&nbsp;
	<?}
	//permisão para excluir lição (sendo coordenador ou que seja criador e não esteja aprovado)
	if($usucpfcoord == $_SESSION['usucpf'] or ($lapstatus == 'P' and ($criador == $_SESSION['usucpf'])))
	{?>
	<img border="0" src="../imagens/excluir.gif" title="Excluir a lição." onclick="excluir_licao('<?=$lapid?>')">
	<?}?>
	</td>	
	<td>
	<?
	//permisão para aprovar lição (sendo coordenador e status = p)
	if ($lapstatus == 'P')
	{?>
	<img border="0" src="../imagens/pendente.gif" title="Aguardando aprovação." onclick="">&nbsp;&nbsp;
	<?}?>
	<?= $lapdescricao?></td>
	<td>
	<?
	 if ($tpasnacerto == 't')
	 {?>
	 	<font color="#009900">Positiva.</font>
	 <?}elseif ($tpasnacerto == 'f')
	 {?>
	 	<font color="#FF0000">Negativa.</font>
	 <?}else{?>
	 	<font color="#0000FF">Não citada.</font> 
	 <?	
	 }
	?>
	</td>
	</tr>
	<? 
			}
	/*fim do foreach $rs*/ $i++;} 
		/*Fim if $rs*/}
		if ($quant_licoes==0) {
	?>
	<tr bgcolor="<?=$marcado?>">
	  <td class="title" style="color:red;">Nenhuma lição aprendida</td>
	  <td></td>
	</tr>
	<? /* Fim else $rs */ } 
	?>
	<tr>
	<td></td>	
	<td><input type="button" value="Inclui nova Lição Aprendida" onclick="inclui_licao()">&nbsp;&nbsp;<input type="button" value="Voltar" onclick="history.back()"></td>
	</tr>
</table>
<?
//Fim if $_REQUEST[ "pjeid" ]
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 

<script>


  function mostra_equipe()
  {
  	 e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/verequipe&acao=A";
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=800,height=300'");
  }

	/**
	* Valida cadastro/ atualização de lição aprendida
	* @return void
	*/
	
	function validar_cadastro()
	{		
		prepara_formulario();
		
		var formulario = document.frmIncluirLicao;
		if( !validaBranco( formulario.titulo, 'Título' ) ) return;
        if( !validaBranco( formulario.lapdescricao, 'Descrição' ) ) return;
        if( !validaBranco( formulario.acpid, 'Área de conhecimento' ) ) return;
        if( !validaBranco( formulario.prpid, 'Processo' ) ) return;
		if( !validaBranco( formulario.interesse, 'Áreas de interesse') ) return;
		if( !validaBranco( formulario.influencia, 'Áreas de influência') ) return;
		if( formulario.acerto_check[ 0 ].checked )
		{
			acerto = formulario.acerto_check[ 0 ].value
		}
		else if( formulario.acerto_check[ 1 ].checked )
		{
			acerto = formulario.acerto_check[ 1 ].value
		}
		else
		{
			alert( "Foi um acerto?" );
			return;
		}
		formulario.acerto.value = acerto;
		formulario.submit();
	}
	
	function altera_licao( lapid )
	{
		
		document.formulario.lapid.value = lapid;
		document.formulario.act2.value = "atualizar";
		document.formulario.submit();
	}
	function excluir_licao( lapid )
	{
		
		if (confirm("Deseja excluir está lição?"))
		{
			document.formulario.lapid.value = lapid;
			document.formulario.act2.value = "excluir";
			document.formulario.submit();
		}
	}
	
	
	function inclui_licao()
	{
		
		document.location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/projespec/licaoaprendida&acao=A&act=inserir&pjeid=<?=$pjeid?>";
		
	}
	
	
	
</script>