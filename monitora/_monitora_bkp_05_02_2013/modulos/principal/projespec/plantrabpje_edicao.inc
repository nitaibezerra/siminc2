<?
/*
 Sistema Simec
 Setor responsavel: SPO-MEC
 Desenvolvedor: Equipe Consultores Simec
 Analista: Gilberto Arruda Cerqueira Xavier
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Fabrício Mendonça(fabriciomendonca@gmail.com)
 Modificacoes: Thiago Mata thiago_mata@yahoo.com.br 9 de novembro de 2006
 Módulo:plantrabpje_insercao.inc
 Finalidade: este arquivo inclui os itens básicos de uma tarefa
 */

require_once( 'plantrabpje_funcoes.inc' );
$pjeid=$_SESSION['pjeid'];

$statusprojeto= projetoaberto();
if ($_REQUEST['act']=='alterar')
{	
	$id = $_REQUEST['ptoid'];
    $id_antecessor=encontra_antecessor($_REQUEST['coantecessoras'],$id);
	$ptodsc = trim(str_to_upper($_REQUEST['ptodsc']));
	$ptodescricao = trim($_REQUEST['ptodescricao']);
	$ptoavisoantecedencia = $_REQUEST['ptoavisoantecedencia'];
	$ptodata_ini = $_REQUEST['ptodata_ini'];
	$ptodata_fim = $_REQUEST['ptodata_fim'];
	$ptosndatafechada = $_REQUEST['ptosndatafechada'];
	$procod = $_REQUEST['procod'];
	$meta = $_REQUEST['ptoprevistoexercicio'];
	$unmcod = $_REQUEST['unmcod'];
	$metavalor = $_REQUEST['ptovlrprevisto'];
	$usucpf = $_REQUEST['usucpf'];
	$id_pai = $_REQUEST['ptoid_pai'];
	$pjeid = $_SESSION['pjeid'];
	$ptocod = trim(str_to_upper($_REQUEST['ptocod']));
	$posicao = $_REQUEST['ptoordem_prov'];
	require_once(APPRAIZ.'includes/planodetrabalho/tarefa_pt/arquivos_requeridosPT.inc' );

	try
	{
		$objTarefa = new TarefaPT();
		$objTarefa = $objTarefa->getTarefaPeloId( $id);
		$objTarefa->setCodigoUnico( $ptocod );
		$objTarefa->setContainerId( $id_pai );
		$objTarefa->setDataFechada( $ptosndatafechada );
		$objTarefa->setNovaDataInicio( $ptodata_ini );
		$objTarefa->setNovaDataFim( $ptodata_fim );
		$objTarefa->setDescricao( $ptodescricao );
		$objTarefa->setNome( $ptodsc );
		$objTarefa->setPosicao( $posicao );
		$objTarefa->setPredecessoraId( $id_antecessor );
		$objTarefa->setProjetoId( $pjeid );
		$objTarefa->setQtdDiasAntesParaAviso( $ptoavisoantecedencia );
		$objTarefa->setProdutoId( $procod );
		$objTarefa->setPrevisaoMeta( $meta );
		$objTarefa->setUnidadeDeMedidaId( $unmcod );
		$objTarefa->setDonoId( $usucpf );
		
//ALTER TABLE monitora.planotrabalho ADD COLUMN ptovalorprevisto numeric(15, 2);
//COMMENT ON COLUMN monitora.planotrabalho.ptovalorprevisto IS 'Controla o valor financeiro da tarefa';
		
		$objTarefa->valida();
		$objTarefa->atualizaTodasAsTarefas();
		$db->commit();
		//novo_ajusta_ordem();
        ?>
          <script>
                alert ('Operação realizada com sucesso!');
               /// window.opener.location.reload();
        		//self.close();
        		//window.close();
          </script>
        <?
       // $db->close();
	}
	catch( Exception $objError )
	{
		//dbg(2,1);
	// Ocorreu um Erro
	?>
		<script>
			<!-- chama o alert passando a mensagem de erro -->
			alert( "<?= $objError->getMessage() ?>" );
		</script>
	<?
	}
}
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/remedial.js"></script>
<script language="JavaScript" src="../includes/superTitle.js"></script>
<script language="JavaScript" src="plantrabpje_funcoes_js.js"></script>
<script language="JavaScript" src="plantrabpje_funcoes_js2.js"></script>
<script language="JavaScript">
	var limiteProjeto = 0<?=$limiteProjeto?>;
	function rep_img(){}
	
</script>
<script language="JavaScript" src="../includes/dateFunctions.js"></script>
<script language="JavaScript">
	var globalDataFimValida = '';
	function atualizaDataPelaDuracao( objInputDataInicio, objInputDataTermino, objInputIntervalo, strQuemMudara ){
		try{
			var intDuracao	= 0 + forceInt( objInputIntervalo.value );
			
			objDataInicio	= strDateToObjDate( objInputDataInicio.value , 'd/m/Y' , '/' );
			objDataTermino	= strDateToObjDate( objInputDataTermino.value , 'd/m/Y' , '/' );
			switch( strQuemMudara )
			{
				case 'objDataInicio':
				{
					// falta a funcao tira dias
					return;
					break;
				}
				case 'objDataTermino':
				{
					objDataTermino	= moreManyDays( objDataInicio , intDuracao );
					objInputDataTermino.value = objDateToStrDate( objDataTermino, 'd/m/Y' , '/' );
					break;
				}
				case 'objIntervalo':
				{
					intNovaDuracao = Math.ceil(( objDataTermino - objDataInicio )/( 60*60*24*1000 ));
					objInputIntervalo.value = intNovaDuracao;
					break;
				}
				default:
				{
					// ninguem será mudado //
					return;
				}
			}
			if( intNovaDuracao < 0 )
			{
				alert( 'Data Término não pode ser Anterior que Data Início.');
				
				objInputDataTermino.value = globalDataFimValida;
				objInputDataTermino.onchange();
			}
			else
			{	
				globalDataFimValida = objInputDataTermino.value;
			}
		}catch( e )
		{
		}
	}
	
</script>
<?


$sql= "	select ug.ungabrev ||'-'|| pe.pjecod || '  ' || pe.pjedsc as pjedsc 
		from 
		monitora.projetoespecial pe left join unidadegestora ug on ug.ungcod=pe.ungcod 
		where pjeid=$pjeid
		";
$pjedsc= $db->pegaUm($sql);
?>

<link	rel="stylesheet" type="text/css" href="../includes/Estilo.css" >
<link	rel='stylesheet' type='text/css' href='../includes/listagem.css' >
<script language="JavaScript" src="../includes/funcoes.js"></script>

<br>
<?
$ptodsc	=$_REQUEST['ptodsc'];
if ($_REQUEST['id'])  
{
	$ptoid=$_REQUEST['id'];
    $sql = "select * from monitora.planotrabalho where ptoid=$ptoid";
    $dados = $db->recuperar($sql);
    if(is_array($dados)) foreach($dados as $k=>$v) ${$k}=$v;
    $ptocod = (mostracod($ptoid));
}
chamaCabecalho($ptoid);
if ($ptoid_pai)
{
	$sql="select ptosndatafechada as paifechado, 
	to_char(ptodata_ini,'dd/mm/YYYY') as pjedataini,
	to_char(ptodata_fim,'dd/mm/YYYY') as pjedatafim 
	from monitora.planotrabalho 
	where ptoid=$ptoid_pai";
	//dbg($sql,1);
	$datas=$db->pegalinha($sql);
	$pjeinimt	=	$datas['pjedataini'];
	$pjefimmt	=	$datas['pjedatafim'];
	$paifechado	=	$datas['paifechado'];

}

$titulo_modulo = 'Editar a Tarefa';


monta_titulo($titulo_modulo,'');
?>
<div align="center">
	<center>
		<?
		// verifica se á coordenador de ação
		$autoriza = false;
		$coordpje=false;
		$digit=false;

		if ( $db->testa_responsavel_projespec( $_SESSION[ 'pjeid' ] ) ) $coordpje = true;
		// verifica se é digitador

		if ( $db->testa_digitador( $_SESSION[ 'pjeid' ], 'E' ) ) $digit = true;
		
		// verificar se é super-usuário
		if ( $db->testa_superuser() )
		{
			$coordpje = true;
			$_SESSION[ 'coordpje' ] = true;
			$digit = true;
		}

		// verifica se o exercício está aberto para estruturar o simec
		$sql= 	"select prsano 
				from monitora.programacaoexercicio 
				where prsano='".$_SESSION['exercicio']."' and prsstatus='A'";

		$registro=$db->recuperar($sql);

		if(	!$coordpje and !$digit and !$visivel )
		{
		?>
			<script>
       			alert ( 'Você não tem acesso para monitorar o Plano de Trabalho neste Projeto Especial!');
			</script>
		<?
		exit();
		}
	/**
	 * Seleciona o somatório das previsões de desembolso do projeto especial em questão
	 */
	
	$sqlLimite =	" select " .
	    			" sum( coalesce( dpe.dpevalor, 0 ) ) as DESPESA " .
				    " from monitora.planotrabalho p " .
				    " left join monitora.desembolso_projeto dpe on dpe.ptoid = p.ptoid " .
				    " where p.ptostatus='A' and p.ptoid_pai is null";

    if( $ptoid )
    {
    	$sqlLimite .= " and p.ptoid <> ".$ptoid;
    }
    $totalGasto = $db->pegaUm( $sqlLimite );
	    
    //calcula o valor que ainda pode ser utilizado nas atividades do projeto especial
    $limiteProjeto = $pjevlrano - $totalGasto;

?>
<form method="POST" name="formulario" id="formulario">
	<input	type='hidden'	name="modulo"	value="<?=$modulo?>"> 
	<input	type='hidden'	name="ptoid"	value="<?=$ptoid?>">
	<input	type='hidden'	name='act'	>
	<input type='hidden' name='act2'>
	<input type='hidden' name='ptotipo' value='<?=$_REQUEST['ptotipo']?>'>
	<input type='hidden' name='dtini' value="<?=$pjedataini?>">
	<input type='hidden' name='dtfim' value="<?=$pjedatafim?>">
	<input type='hidden' name='dtinimt' value="<?=$pjeinimt?>">
	<input type='hidden' name='dtfimmt' value="<?=$pjefimmt?>">
	<input type='hidden' name='projfechado' value="<?=$projfechado?>">
	<input type='hidden' name='paifechado' value="<?=$paifechado?>">
	<input type='hidden' name='ptosnsoma' value='t'>
	<input type='hidden' name='ptocod' value=="<?=$ptocod?>">
	<input type='hidden' name='ptoid_pai' value="<?=$ptoid_pai?>">
	<input type='hidden' name='ptoordemprov' value="<?=$ptoordemprov?>">
	<input type='hidden' id="1" name='ptovlrprevisto' value="0">
	    <script type="text/javascript">
	    arrMacroEtapas = new Array();
		</script>
	<div id="camposDinamicos" style="visibility:hidden;margin:0; padding:0;"></div>
	<center>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
		align="center" style="color:#808080;">
			<tr>
				<td align='right' class="SubTituloDireita">Projeto :</td>
				<td><b><?=$pjedsc?></b></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Período do Projeto:</td>
				<td><b><?=$pjedataini?>&nbsp; - &nbsp;<?=$pjedatafim?></b></td>
			</tr>
			<?
			if ($ptoid_pai)
			{?>
			<tr>
				<td align='right' class="SubTituloDireita">Período da Tarefa Pai:</td>
				<td><b><?=$pjeinimt?>&nbsp; - &nbsp;<?=$pjefimmt?></b></td>
			</tr>				
			<?}?>
		<tr>
			<td align='right' class="SubTituloDireita">
				Produto :
			</td>
			<td>
				<b>
					<?=$prodsc?>
				</b>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">
				Físico e Financeiro :
			</td>
			<td>
				<b>
					<?=$pjeprevistoano?>&nbsp;/&nbsp;<?='R$ '.number_format($pjevlrano,2,',','.')?>
				</b>
			</td>
		</tr>
		   	<tr>
		<td class="SubTituloDireita">Arquivos vinculados à tarefa:</td>
		<td>
			<? 	
				$sql=sql_vincula_arquivo('monitora.planotrabalho',$ptoid,'ptoid');
				popup_arquivo( 'monitora.planotrabalho', $sql, 'ptoid',$ptoid, 1, 400, 400,'monitora.planotrabalho' );		
					
			?>
		</td>
	</tr>
	<tr>
        <td align='right' class="SubTituloDireita">Código:</td>
		<td>
		<?=$ptocod ?>
		</td>
	</tr>
	<!--
<tr>
        <td align='right' class="SubTituloDireita">Tarefas antecessoras:</td>
	<td>
		<?=campo_texto('coantecessoras','N','S','',30,30,'','','','Entre com o número de ordem da(s) Tarefa(s) antecessora(s) separados por vírgula (Ex: 12,45,76).');?>&nbsp;&nbsp;ATENÇÃO! Entre com o nº de ordem da tarefa.
	</td>
   </tr> 
   --> 
    <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
		<td>
			<?=campo_texto('ptodsc','S','S','',77,80,'','','','Entre com o título (ou nome) que esta Tarefa será conhecida.');?>
		</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>
        <td><?=campo_textarea('ptodescricao','S','S','',80,3,'','','',' Campo utilizado para descrever o motivo da criação da Tarefa.');?></td>
      </tr>  
<tr>
        <td align='right' class="SubTituloDireita">Antecedência de aviso:<br>(Padrão: 7 dias)</td>
	<td>
	<?=campo_texto('ptoavisoantecedencia','N','S','',4,2,'','','','O sistema irá avisá-lo n dias antes do início da Tarefa. Este aviso será visual e por e-mail.');?>
	</td>
      </tr>      
      <tr>
        <td align='right' class="SubTituloDireita">Data Início:</td>
        <td>
        
		<input class="normal" type="text"
		name="ptodata_ini" 
		onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" 
		onmouseover="MouseOver(this);" 
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objDataTermino' )" 
		onkeyup="this.value=mascaraglobal('##/##/####',this.value);" 
		value="<?=  formata_data( $ptodata_ini )  ?>" 
		maxlength="10" 
		style="text-align: right;" size="12"/>
		        
		<a href="javascript:show_calendar('formulario.ptodata_ini');">
			<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt="">
		</a>
				
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Duração:</td>
        <td>
		<input type="text" 
		name="duracao"
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objDataTermino' )" size="3" maxlength="3" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"/> dias
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Data Término:</td>
        <td>
        
		<input 
		name="ptodata_fim"
		class="normal" type="text" 
		onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="globalDataFimValida=this.value;MouseClick(this);" 
		onmouseover="MouseOver(this);" 
		onchange="atualizaDataPelaDuracao( document.formulario.ptodata_ini , document.formulario.ptodata_fim, document.formulario.duracao , 'objIntervalo' )" 
		onkeyup="this.value=mascaraglobal('##/##/####',this.value);" 
		value="<?= formata_data( $ptodata_fim ) ?>" 
		maxlength="10" 
		style="text-align: right;" size="12"/>
		
		<a href="javascript:show_calendar('formulario.ptodata_fim');">
			<img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt="">
		</a>		

	</td>
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita">Congela as datas?</td>
        <td>
            <input type="radio" name="ptosndatafechada" value="t" <?=($ptosndatafechada=='t'?"CHECKED":"")?>>  Sim
                &nbsp;<input type="radio" name="ptosndatafechada" value="f" <?=($ptosndatafechada=='f'?"CHECKED":"")?>> Não
         </td>
       </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Produto:</td>
	    <td><?
	    $sql = "select procod as CODIGO,prodsc as DESCRICAO from produto where prostatus='A' order by prodsc ";
	    $db->monta_combo("procod",$sql,'S',"Selecione o Produto",'','','',400);
	    ?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Meta:</td>
	<td><?=campo_texto('ptoprevistoexercicio','S','S','',16,14,'#########','');?>
	</td>
      </tr>
	        <tr>      
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td >
	<?
	$sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A' order by unmdsc ";
	$db->monta_combo("unmcod",$sql,'S',"Selecione a Unidade de Medida",'','','Entre com a Unidade de Medida da Tarefa. Campo obrigatório!',400,'S');
	?>
	</td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Financeiro:</td>
	<td><?=campo_texto('ptovalorprevisto','N','S','',16,14,'###.###.###,##','');?>
	</td>
      </tr>    
      <tr>
        <td align='right' class="SubTituloDireita">Responsável:</td>
	    <td><? $sql = "select distinct u.usucpf as CODIGO,u.usucpf ||'-'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as DESCRICAO, u.usunome from seguranca.usuario u inner join monitora.usuarioresponsabilidade ur on ur.usucpf=u.usucpf and ur.pjeid=$pjeid where u.usucpf in (select pu.usucpf from seguranca.perfilusuario pu where pu.pflcod=51) or u.usucpf in (select us.usucpf from seguranca.usuario_sistema us where us.pflcod=51) and u.usustatus='A' order by usunome ";
	$db->monta_combo("usucpf",$sql,'S',"Selecione o Responsável",'','','',400);
	    ?></td>
      </tr>  
      <tr bgcolor="#CCCCCC">
   <td></td>
   <td>
      <? if ($statusprojeto) {?>
   <input type="button" name="btgravar" value="Gravar" onclick="validar_alterar()" class="botao">&nbsp;&nbsp;
   <?}?>
   
   <input type="button" name="btvoltar" value="Cancelar" onclick="window.close();" class="botao"></td>

 </tr>
	</tbody>
</table>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 

</center>
</div>
</form>

<script>
  function validar_alterar()
  {

  	if (!validaBranco(document.formulario.ptodsc, 'Título')) return;
	if (!validaBranco(document.formulario.ptodescricao, 'Descrição')) return;		
	if (!validaBranco(document.formulario.usucpf, 'Responsável')) return;		
  	
  	document.formulario.act.value='alterar';
  	 document.formulario.submit();
  }
  window.focus();
</script>
