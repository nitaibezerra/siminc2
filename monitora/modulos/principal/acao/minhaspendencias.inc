

<?
//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";
?>

<br>
<?

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Minhas pendências','Clique na imagem para entrar com o monitoramento.');
		// controle de plano de trabalho
		// nas ações o plano de trabalho deve ser acompanhado conforme a data de inicio e o aviso de antecedencia
		// ou seja, para cada atividade verificar a data de inicio menso a antecednecia e ver se está na hora de avisar, desde que a atividade esteja aprovada .
	$sql = "select a.acaid,a.prgcod||'-'||a.acacod||'  '||a.acadsc as descricao from monitora.acao a inner join monitora.usuarioresponsabilidade ur ON ur.acaid = a.acaid where a.prgano = '".$_SESSION['exercicio']."' and acasnrap='f' and ur.usucpf = '".$_SESSION['usucpf']."' order by acacod";	
    $acoes = $db->carregar ($sql);
   $ok=0;
    if (is_array($acoes))
    {
        foreach ($acoes as $acao)
        {
		  //  $sql = "select '<img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Acompanhar Tarefa.\" onclick=\"editarpendencia('||pg.ptoid||')\">',pg.ptodsc from monitora.planotrabalho pg inner join monitora.acao pe on pe.acaid=pg.acaid and pe.acastatus='A' where (pg.ptodata_ini <= current_date-pg.ptoavisoantecedencia) and pg.ptostatus='A' and pe.acaid=".$acao['acaid']." and pg.usucpf='".$_SESSION['usucpf']."' and pg.ptosnaprovado='t' and pg.ptoid not in (select ptoid from monitora.execucaopto where acaid=".$acao['acaid']." and ptoid is not null) order by ptoordem";
		    $sql = "select '<img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Acompanhar Tarefa.\" onclick=\"editarpendencia('||pg.ptoid||')\">',pg.ptodsc from monitora.planotrabalho pg inner join monitora.acao pe on pe.acaid=pg.acaid and pe.acastatus='A' where (pg.ptodata_ini <= current_date-pg.ptoavisoantecedencia) and pg.ptostatus='A' and pe.acaid=".$acao['acaid']." and pg.ptosnaprovado='t' and pg.ptoid not in (select ptoid from monitora.execucaopto where acaid=".$acao['acaid']." and ptoid is not null) and pg.usucpf='".$_SESSION['usucpf']."' order by ptoordem";		    
			
	       $rs=$db->carregar($sql);
	   		if (  $rs && count($rs) > 0 )
			{	
	?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr align="left"> <td><b>AÇÃO: <?=$acao['descricao']?></td></tr></table>
	<?

	$cabecalho = array( 'Monitorar', 'Título da tarefa' );
			$db->monta_lista( $sql, $cabecalho, 50, 20, '', '' ,'' );	
			$ok=1;
			}
        }
    }
    if (! $ok)
    {
    ?>
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr align="left"> <td><b><font color="red">NÃO HÁ PENDÊNCIAS</td></tr></table>
	<?
    }

		
?>
<script>
function editarpendencia(cod)
{

	 e = "monitora.php?modulo=principal/acao/editartarefa2&acao=I&ptoid="+cod;
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'");

}
</script>
