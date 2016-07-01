<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:cadacao.inc
   Finalidade: permitir o controle do cadastro de acao
   */

$modulo=$_REQUEST['modulo'] ;//

if (!$_REQUEST['prgid'] && $_REQUEST['acaid']){
	$_SESSION['prgid'] = $db->pegaUm("SELECT prgid FROM monitora.acao WHERE acaid = {$_REQUEST['acaid']}");
	$_REQUEST['prgid'] = $_SESSION['prgid'];
}


if ($_REQUEST['prgid']) 
	$_SESSION['prgid'] = $_REQUEST['prgid'];
else if ($_SESSION['prgid']) 
	$_REQUEST['prgid'] = $_SESSION['prgid'];



if ($_REQUEST['acaid']) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] =$_SESSION['acaid'];

$sql = "select a.*, b.prgdsc from acao a, programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.prgid=".(integer)$_SESSION['prgid'];

if ($_REQUEST['acaid'])
{
    $sql = $sql. "  and a.acaid = ".(integer)$_REQUEST['acaid'];
}

// se houver prgid busca então usá-lo
if ($_REQUEST['prgcodbusca'])
{
  $sql = "select a.*, b.prgdsc from monitora.acao a, monitora.programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.prgcod='".$_REQUEST['prgcodbusca']."'";
}
// se houver acacodbusca busca então usá-lo
if ($_REQUEST['acacodbusca'])
{
  $sql = "select a.*, b.prgdsc from acao a, programa b where a.acastatus='A' and b.prgstatus='A' and a.prgid = b.prgid and a.prgano ='" .$_SESSION['exercicio']."' and a.acacod='".str_to_upper($_REQUEST['acacodbusca'])."'";
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

$nl=$nlinhas;
if ($nlinhas >= 0) {


   // as linhas abaixo não permitem navegar para além dos limites

// fim do controle de navegação
    $res =  $db->carrega_registro($RS,$_POST['registro']);
// a linha abaixo transforma em variÃ¡veis todos os campos do array
    if(is_array($res)){
       foreach($res as $k=>$v) {${$k}=$v;};
            $_SESSION['prgid'] = $prgid;
            $_SESSION['acaid']= $acaid;
            if ($acasnmetanaocumulativa=='t') $tipometa ='Não cumulativa';
            if ($acasnmetanaocumulativa=='f') $tipometa ='Cumulativa';
            if ($acasnmetanaocumulativa=='') $tipometa ='Não informado';
       }
	// encontrar o desafio (macro-objetivo)
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o da unidade de medida
        $sql= "select unmdsc from public.unidademedida where unmcod='".$unmcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar descriÃ§Ã£o do tipo de aÃ§Ã£o
        $sql= "select tacdsc from public.tipoacao where taccod='".$taccod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // encontrar a funÃ§Ã£o
       if ($funcod) {
        $sql= "select fundsc from ppafuncao where funcod='".$funcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
        // encontrar a subfunÃ§Ã£o
       if ($sfucod) {
        $sql= "select sfudsc from ppasubfuncao where sfucod='".$sfucod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       }
    // encontrar o Ã³rgÃ£o responsÃ¡vel
        $sql= "select orgdsc from orgao where orgcod='".$orgcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // encontrar o orgao responsÃ¡vel
        $sql= "select orgdsc from orgao where substr(orgcod,0,3)='".substr($unicod,0,2)."'";

        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        
    // encontrar a unidade responsÃ¡vel
        $sql= "select unicod||'-'||unidsc as unidade from unidade where unicod='".$unicod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar o produto
        $sql= "select prodsc from produto where procod='".$procod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a meta
        $sql= "select fisqtdeprevistoano as meta from dadofisico where acacod='$acacod' and saccod='$saccod' and regcod='$regcod' and prgcod='$prgcod' and prgano='".$_SESSION['exercicio']."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    // encontrar a esfera
       if ($esfcod) {
        $sql= "select esfdsc from esfera where esfcod='".$esfcod."'";
        $res = $db->recuperar($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        }
 } else
 {
   ?>
<script>
    alert('Código da Ação inexistente.! \nVocê pode estar tentando abrir ação de outro exercício.');
    history.back();
</script>
   <?
   exit();
 }

 if ($_REQUEST['prgid']) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] =$_SESSION['prgid'];
if ($_REQUEST['acaid']) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] =$_SESSION['acaid'];

include  APPRAIZ."includes/cabecalho.inc";		
?>
<br>
<? 
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Atributo da Ação';
monta_titulo($titulo_modulo,'');
?>
<? // inclusão do menu de setas de navegação
$nlinhas=$nl;
?> 
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="orgao" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="navega" value=0>
<input type=hidden name="procura" value=0>
<!--
<tr><td align='right' class="SubTituloDireita">Programa</td>
    <td><?=campo_texto('prgcod','S',$habil,'',8,4,'','');?>
	<img border="0" src="imagens/busca.gif"  alt='Procura programa' title='Procura programa' onclick="ProcuraPrograma(this.form)">
    <?='  '.$prgdsc?> 
    </td></tr>-->
	 <tr>
        <td align='right' class="SubTituloDireita" style="width: 20%">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?><!--<input type="text" name="acacodbusca" size="5" ><img border="0" src="imagens/busca.gif"  alt='Procura ação' title='Procura ação' onclick="ProcuraAcao(this.form)">-->
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Ação:</td>
        <td><b><?=$acacod?></b>.<?=$unicod.'.'.$loccod?><!--<input type="text" name="acacodbusca" size="5" ><img border="0" src="imagens/busca.gif"  alt='Procura ação' title='Procura ação' onclick="ProcuraAcao(this.form)">-->
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
        <td><?=campo_textarea('acadsc','N','N','',100,3,'');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão Orçamentário Responsável:</td>
        <td><?=campo_texto('orgdsc','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade Orçamentária Responsável:</td>
        <td><?=campo_texto('unidade','N','N','',100,100,'','');?></td>
      </tr>
      <?  if ($fundsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Função:</td>
        <td><?=campo_texto('fundsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <? if ($sfudsc) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Subfunção:</td>
        <td><?=campo_texto('sfudsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
      <? if ($acafinalidade) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
        <td><?=campo_textarea('acafinalidade','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
      <? if ($acadescricao) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>  
        <td><?=campo_textarea('acadescricao','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
    <? if ($prodsc) {?>
       <tr>
        <td align='right' class="SubTituloDireita">Produto (Bem ou Serviço):</td>
        <td><?=campo_texto('prodsc','N','N','',100,100,'','');?></td>
      </tr>
      <?}?>
       <tr>
        <td align='right' class="SubTituloDireita">Meta (<b><?=$tipometa?>)</b>:</td>
        <td><?=campo_texto('meta','N','N','',100,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><?=campo_texto('unmdsc','N','N','',100,100,'','');?></td>
      </tr>

      <? if ($acadscproduto) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Especificação do Produto:</td>
        <td><?=campo_textarea('acadscproduto','N','N','',100,5,'');?></td>
      </tr>
      <?}?>
  <?
    if ($acasnoutrasfontes =='t') {?>
        <tr>
        <td align='right' class="SubTituloDireita">Quanto ao Orçamento:</td>
        <td><input type="hidden" name="taccod" value='3'>Não orçamentária</td>
      </tr>
   <? } else {?>
        <tr>
           <td align='right' class="SubTituloDireita">Quanto ao Orçamento:</td>
           <td><input type="hidden" name="taccod" value='3'>Orçamentária</td>
        </tr>
        <tr>
           <td align='right' class="SubTituloDireita">Tipo de Orçamento:</td>
           <td>
	    <?
	         if ($acasnfiscalseguridade=='t') print 'Fiscal&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acasnfiscalseguridade=='f') print 'Seguridade&nbsp;&nbsp;&nbsp;&nbsp;';
  	         if ($acasninvestatais=='t') print 'Investimentos Estatais&nbsp;&nbsp;&nbsp;&nbsp;';
	    ?>
          </td>
      </tr>
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de ação:</td>
         <td><input type="hidden" name="tacdsc" ><?=$tacdsc?></td>
      </tr>
      <?if ($taccod==1){?>
        <tr>
	      <td align='right' class="SubTituloDireita">Evolução da Situação Física:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='33%'>Total</td>
                   <td width='33%'>Realizado até <?=strval($_SESSION['exercicio'])-2?></td>
                   <td width='33%'>Previsto em <?=$_SESSION['exercicio']-1?></td>
                </tr>
                <tr>
                   <td width='33%'><?=$acaqtdcustototal?></td>
                   <td width='33%'><?=$acaqtdateanoanterior?></td>
                   <td width='33%'><?=$acaqtdprevistoanocorrente?></td>
                </tr>
             </table>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Evolução da Situação Financeira:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='33%'>Total</td>
                   <td width='33%'>Realizado até <?=strval($_SESSION['exercicio'])-2?></td>
                   <td width='33%'>Previsto em <?=$_SESSION['exercicio']-1?></td>
                </tr>
                <tr>
                   <td width='33%'><?=number_format($acavlrcustototal,2,',','.')?></td>
                   <td width='33%'><?=number_format($acavlrcustoateanoanterior,2,',','.')?></td>
                   <td width='33%'><?=number_format($acavlrprevistoanocorrente,2,',','.')?></td>
                </tr>
             </table>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Justificativa da Repercussão Financeira sobre o Custeio da União:</td>
          <td>
             <?=campo_textarea('acarepercfinanceira','N','N','',100,5,'');?>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Valor Estimado da Repercussão Financeira (R$/ano):</td>
          <td>
             <?=number_format($acavlrrepercfinanceira,2,',','.')?>
          </td>
         </tr>
        <tr>
	      <td align='right' class="SubTituloDireita">Duração do Projeto:</td>
          <td>
             <table border=1>
                <tr>
                   <td width='50%'>Início</td>
                   <td width='50%'>Término</td>
                </tr>
                <tr>
                   <td width='50%'><?=$acamesinicio.'/'.$acaanoinicio?></td>
                   <td width='50%'><?=$acamestermino.'/'.$acaanotermino?></td>
                </tr>
             </table>
          </td>
         </tr>
      <?}?>
      <? if ($esfcod) {?>
        <tr>
	      <td align='right' class="SubTituloDireita">Esfera:</td>
          <td>
             <?=campo_texto('esfdsc','N','N','',50,50,'','');?>
          </td>
         </tr>
       <?}?>
      <tr>
	     <td align='right' class="SubTituloDireita">Forma de Implementação:</td>
         <td><input type="hidden" name="tacdsc" >
         <?
            if ($acasnmedireta=='t') print 'Direta<br>';
            if ($acasnmedesc=='t')   print 'Descentralizada<br>';
		    if ($acasnmelincred=='t') print 'Linha de Crédito<br>';
            if ($acasntransfobrigatoria=='t') print 'Transferência obrigatória<br>';
            if ($acasntransfvoluntaria=='t') print 'Transferência voluntária<br>';
            if ($acasntransfoutras=='t') print 'Outras transferências<br>';
	    
         ?>
         </td>
      </tr>
      <? if ($acadetalhamento) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Detalhamento da Implementação:</td>
        <td><?=campo_textarea('acadetalhamento','N','N','',100,3,'');?></td>
      </tr>
      <?}?>
      <?}?>
     
      <?
    if ($acabaselegal ) {?>
      <tr>
        <td align='right' class="SubTituloDireita">Base legal:</td>
        <td><?=campo_textarea('acabaselegal','N','N','',100,3,'');?></td>
	</td>
      </tr>
     <?}?>
      <? if ($acahistorico) {?>
     <tr>
        <td align='right' class="SubTituloDireita">Histórico da ação:</td>
       <td><?=campo_textarea('acahistorico','N','N','',100,5,'');?></td>
      </tr>
     <?}?>
	<?$db -> mostra_resp($_SESSION['acaid'], 'acaid');?>
<? if ($db->testa_uma()) { ?>
  <tr bgcolor="#cccccc">
      <td></td>
   <td><input type="button" class="botao" name="btassociar" value="Associar Responsáveis" onclick="Associa_resp()"></td>
      </tr>
  <?}?>
</form>



		<?php
		
		/*
		
		create table monitora.acaounidadegestora (
			acaid int4 not null,
			ungcod char(6) not null,
			constraint pk_acaounidadegestora primary key ( acaid, ungcod ),
			constraint fk_acaounidadegestora_acaid foreign key ( acaid )
				references monitora.acao ( acaid ) match simple
				on update cascade
				on delete restrict,
			constraint fk_acaounidadegestora_ungcod foreign key ( ungcod )
				references public.unidadegestora ( ungcod ) match simple
				on update cascade
				on delete restrict
		) with oids;
		
		*/
		
		$sql = sprintf(
			"select count( pflcod )
			from seguranca.perfilusuario pu
			where pu.usucpf = '%s' and  pu.pflcod in ( 18, 112, 6 )",
			$_SESSION['usucpf']
		);
		$privilegio_associacao = $db->pegaUm( $sql ) > 0;
		$ungcod_habil = $privilegio_associacao ? 'S' : 'N';
		
		if ( $privilegio_associacao ) {
            
			# grava a associação
			if ( $_REQUEST['unidadegestora'] ) {
				$sql = sprintf( "delete from monitora.acaounidadegestora where acaid = %d", $_SESSION['acaid'] );
				$db->executar( $sql );
				if($_REQUEST['ungcod'][0]) {
				foreach ( (array) $_REQUEST['ungcod'] as $ungcod ) 
                {
                    if(count($_REQUEST['ungcod'])>0)
                    {
                        $sql = sprintf(
                            "insert into monitora.acaounidadegestora ( ungcod, acaid ) values ( '%s', %d )",
                            $ungcod,
                            $_SESSION['acaid']);
                        $db->executar( $sql );
                    }
				}
				$db->commit();
			}
			}
		}
		
		$sql = sprintf(
			"select u.ungcod as codigo, u.ungdsc as descricao
			from monitora.acaounidadegestora aug
			inner join public.unidadegestora u on u.ungcod = aug.ungcod
			where aug.acaid = %d",
			$_SESSION['acaid']
		);
		$ungcod = $db->carregar( $sql );
		
		?>
		<form method="post" name="unidadegestora">
			<input type="hidden" name="unidadegestora" value="1"/>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
					<td>
						<?php
						$sql = sprintf( "select ungcod as codigo, ungdsc as descricao from public.unidadegestora order by ungdsc" );
						combo_popup( "ungcod", $sql, "Unidade Gestora", "400x400", 0, array(), "", $ungcod_habil, false, false, 5, 400 );
						?>
					</td>
				</tr>
				<?php if( $privilegio_associacao ): ?>
					<tr bgcolor="#cccccc">
						<td style="width: 20%;">&nbsp;</td>
						<td>
							<input type="button" class="botao" name="" value="Gravar" onclick="associar_ug();"/>
						</td>
					</tr>
				<?php endif; ?>
			</table>
  	  </form>
    <script>
    	
	    function selectAllOptions(obj) {
			for (var i=0; i<obj.options.length; i++) {
				obj.options[i].selected = true;
			}
		}
		
    	function associar_ug(){
    		selectAllOptions( document.getElementById( 'ungcod' ) );
			document.unidadegestora.submit();
    	}
    </script>

<script>
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
	function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
    function VerRegistro(cod) {
	//alert (cod+1);
        document.formulario.navega.value = cod;
	document.formulario.submit();
    }
    function AvaliaAcao(cod) {
	document.formulario.avalia.value = cod;
	document.formulario.submit();
    }
    function ProcuraAcao(form) {
	document.formulario.procura.value = document.formulario.acacodbusca.value;
	document.formulario.submit();
    }
    function ProcuraPrograma(form) {
	document.formulario.submit();
    }
    function Associa_resp()
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/assocrespac&acao=A";
          WindowObjectReference = window.open(e, "Associação_de_Responsáveis",
"menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
    }
    

</script>

