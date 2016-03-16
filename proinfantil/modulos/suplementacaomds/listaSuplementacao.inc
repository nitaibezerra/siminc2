<?php 

unset($_SESSION['proinfantil']['mds']['entcodent']);
unset($_SESSION['proinfantil']['mds']['mdsid']);

if(!$_SESSION['proinfantil']['mds']['muncod']){
	
	$sql = "select 
				muncod 
			from 
				proinfantil.usuarioresponsabilidade 
			where 
				usucpf = '{$_SESSION['usucpf']}' 
			and 
				pflcod = ".EQUIPE_MUNICIPAL;
	
	$muncod = $db->pegaUm($sql);
	
	if($muncod){
		$_SESSION['proinfantil']['mds']['muncod'] = $muncod;
	}
}

if(!$_SESSION['proinfantil']['mds']['muncod'] && !$db->testa_superuser()){
	
	echo "<script>
			alert('Usuário sem responsabiliade!');
			document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
		  </script>";	
}

if($_POST){
	extract($_POST);
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$abacod_tela = 57559;
$arMnuid = array(11773, 11772, 11771);

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Lista de Suplementações MDS', 'Filtros');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
$(function(){

	$('#btn_salvar').click(function(){
		$('#formulario').submit();
	});

	$('#btn_limpar').click(function(){
		document.location.href = 'proinfantil.php?modulo=suplementacaomds/listaSuplementacao&acao=A';
	});
	
});
function editarEscola( entcodent )
{	
	document.location.href = 'proinfantil.php?modulo=suplementacaomds/questionarioSuplementacao&acao=A&entcodent='+entcodent;
}
//-->
</script>
<form method="post" name="formulario" id="formulario" action="">
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita">Nome da escola</td>
			<td>
				<?php echo campo_texto('entnome', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btn_salvar" />
				<input type="button" value="Limpar" id="btn_limpar" />
			</td>
		</tr>
	</table>
</form>
<?php 
$arWhere = array();
if($_SESSION['proinfantil']['mds']['muncod']){
	$arWhere[] = "mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";
}

if($_REQUEST['entnome']){
	$arWhere[] = "ent.entnome ilike '%{$_REQUEST['entnome']}%'";
}

$sql = "select distinct
			'
			<img src=\"../imagens/alterar.gif\" onclick=\"editarEscola(' || ent.entcodent || ')\" style=\"cursor:pointer;\" />
			' as acao,
			mun.estuf,
            mun.mundescricao,
            ent.entcodent,
            ent.entnome,
            pro.prcqtdalunoinfantil,

            case  when       ent.tplid     = 1 then 'Urbana'
                  when       ent.tplid     = 2 then 'Rural' 
            end  as localizacao,

            case  when       ent.tpcid = 1 then 'Federal'
                  when       ent.tpcid = 2 then 'Estadual'
                  when       ent.tpcid = 3 then 'Municipal'
                  when       ent.tpcid = 4 then 'Privada'
            end as rede
        from
            entidade.entidade ent
        inner join
            entidade.endereco ede on ede.entid = ent.entid
        inner join
            territorios.municipio mun on mun.muncod = ede.muncod
        inner join 
        	proinfantil.procenso pro on pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
		where 
			ent.entstatus = 'A'
		and 
			ent.entcodent is not null
		AND 
			ent.tpcid in (1,2,3) ".		($arWhere ? ' AND '.implode(' AND ', $arWhere) : '')."";
//ver($sql, d);		
		
$cabecalho = array('Ação', 'UF', 'Município', 'INEP', 'Escola', 'Qtd. Matrícula', 'Localização', 'Rede');
$db->monta_lista($sql, $cabecalho, 10, 10, 'N', '', '', '', array(80, null, 180));

?>