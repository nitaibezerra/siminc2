

<?
//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";
?>

<br>
<?

$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Minhas pendências','Clique na imagem para entrar com o monitoramento.');
		// controle de plano de trabalho
		// nos projetos o plano de trabalho deve ser acompanhado conforme a data de inicio e o aviso de antecedencia
		// ou seja, para cada atividade verificar a data de inicio menso a antecednecia e ver se está na hora de avisar, desde que a atividade esteja aprovada e o projeto esteja em execução.
	//$sql = "select p.pjeid,ug.ungabrev||'-'||p.pjecod||'  '||p.pjedsc as descricao from monitora.projetoespecial p inner join unidadegestora ug using (ungcod) order by pjeid";
	$sql = "select p.pjeid,ug.ungabrev||'-'||p.pjecod||'  '||p.pjedsc as descricao from monitora.projetoespecial p inner join unidadegestora ug using (ungcod) where p.tpscod=11 order by pjeid";	
    $projetos = $db->carregar ($sql);
    if (is_array($projetos))
    {
        foreach ($projetos as $projeto)
        {
		    $sql = "select '<img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Acompanhar Tarefa.\" onclick=\"editarpendencia('||pg.ptoid||')\">',pg.ptodsc from monitora.planotrabalho pg inner join monitora.projetoespecial pe on pe.pjeid=pg.pjeid and pe.pjestatus='A' where (pg.ptodata_ini <= current_date-pg.ptoavisoantecedencia) and pg.ptostatus='A' and pe.pjeid=".$projeto['pjeid']." and pg.usucpf='".$_SESSION['usucpf']."' and pg.ptosnaprovado='t' and pg.ptoid not in (select ptoid from monitora.execucaopto where pjeid=".$projeto['pjeid']." and ptoid is not null) order by ptoordem";	
	        $rs=$db->carregar($sql);
	   		if (  $rs && count($rs) > 0 )
			{		
	?>
				<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr align="left"> <td><b>PROJETO: <?=$projeto['descricao']?></td></tr></table>
	<?
			$cabecalho = array( 'Monitorar', 'Título da tarefa' );
			$db->monta_lista( $sql, $cabecalho, 50, 20, '', '' ,'' );	
			}
		}
    }
    else 
    {
    	?>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr align="left"> <td><b><font color="red">NÃO HÁ PENDÊNCIAS EM PROJETOS</td></tr></table>
	<?
    }
		
?>
<script>
function editarpendencia(cod)
{
	 e = "monitora.php?modulo=principal/projespec/editapje&acao=I&ptoid="+cod;
     window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=500'");

}

</script>
