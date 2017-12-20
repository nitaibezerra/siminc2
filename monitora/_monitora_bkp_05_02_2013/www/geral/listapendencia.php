<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listamacroetapa.php
   Finalidade: permitir a listagem inteligente das etapas e macro-etapas
   */

function projetoaberto()
 {
 	 global $db;
	 
 	// verifica se o projeto esta aberto para ser acompanhado,	//
 	// ou seja, se ele nao esta concluido, cancelado etc.		//
 	
	/*
	"1",Atrasado
	"2",Cancelado
	"3",Concluido
	"4",Em dia
	"5",Nao iniciado
	"6",Paralisado
	"7",Suspenso"
	"8",Sem andamento
	"9",Iniciado"
	10 - fase de planejamento
	*/
	
	$sql = ' SELECT ' .
				'tpscod' .
			' FROM ' .
				'monitora.projetoespecial' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
				
	$sit=$db->pegaUm($sql);
	if ($sit=='1' or $sit=='4' or $sit=='9' or $sit=='6' or $sit=='8' or $sit=='10')
	{
		// se o projeto estiver atrasasdo, ou em dia, ou iniciado, ou sem andamento ou paralisado ent	//
		// pode acompanhar																					//
		return true;
	}
//	else
	{
		return false;
	}
}

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$ptoid=$_SESSION['ptoid'];
$pjeid=$_SESSION['pjeid'];
$dadosRequest = $_SESSION[ 'request' ];
$erroData = $_SESSION[ 'erroData' ];
$arrCodigos = explode( ',', $dadosRequest[ 'arrCod' ] );

$coordpje = $_SESSION['coordpje'];
$intPixelPasso = 20;


		// controle de plano de trabalho
		// nos projetos o plano de trabalho deve ser acompanhado conforme a data de inicio e o aviso de antecedencia
		// ou seja, para cada atividade verificar a data de inicio menso a antecednecia e ver se está na hora de avisar, desde que a atividade esteja aprovada e o projeto esteja em execução.
		$sql = "select pe.pjedsc,pe.pjeid,pg.ptoid, pg.ptocod||'-'||pg.ptodsc as descricao,pg.ptosntemdono as temdono,pg.usucpf as dono, to_char(pg.ptodata_ini,'YYYY-MM-DD') as ptodata_ini,pg.ptoavisoantecedencia from monitora.planotrabalho pg inner join monitora.projetoespecial pe on pe.pjeid=pg.pjeid and pe.pjestatus='A' and pe.tpscod in (11) where pg.ptostatus='A' and pg.ptosnaprovado='t'";		
        $rs=$db->carregar($sql);
   		if (  $rs && count($rs) > 0 )
		{			
			// verifico quem é o coordenador
	 		foreach ( $rs as $linha )
			{
		 		foreach($linha as $k=>$v) ${$k}=$v;
		 		$sql = "select u.usucpf as cpfcoord, u.usuemail as emailcoord from monitora.projetoespecial pe inner join monitora.usuarioresponsabilidade ur on ur.pjeid=pe.pjeid and ur.pflcod=47 inner join seguranca.usuario u on u.usucpf=ur.usucpf where pe.pjeid=$pjeid";
			   $resw=$db->pegaLinha($sql);
			   foreach($resw as $k=>$v) ${$k}=$v;
		 		
    	 		if (strtotime($ptodata_ini) <= strtotime(date('Y-m-d')) - $ptoavisoantecedencia)
		 		{	 			
		 		// para cada item do plano de trabalho que pode ser monitorado verifico se já foi feito algum acompanhamento dentro do período de antecedencia
                $sql = "select expid from monitora.execucaopto where ptoid=$ptoid ";
              
                if (! $db->pegaUm($sql))
                {
                	// então precisa avisar
                	// para avisar precisa ver se pode avisar a qualquer um ou a apenas o coordenador                    	// verifica se a mensagem já foi criada
                	$sql = "select msgid from mensagem where msgconteudo = 'Acompanhar a atividade $descricao no Projeto $pjedsc'";

                 	if (! $db->pegaUm($sql))
                 	{
                 		// neste ponto deve ser criada uma mensagem para os usuários envolvidos
                	$sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Acompanhamento de atividade em Projeto','Acompanhar a atividade $descricao no Projeto $pjedsc','00000000191',6)";
		 				$saida=$db->executar($sql);
            			$sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);		 			    
            			$ultimomsgid = $db->pegaUm( $sql );
                		$usuemails='';
						$sql = "insert into mensagemusuario (msgid,usucpf) values ($ultimomsgid,'$cpfcoord')";
						$saida=$db->executar($sql);
		 				$usuemails .= $emailcoord.',';                	 
                		if ($temdono=='t' and $dono)
                		{
                	  			// avisa ao coordenador e ao dono
		 					$sql = "select u.usuemail as emaildono from seguranca.usuario u where u.usucpf='$dono'";
							$emaildono=$db->pegaUm($sql);
							$sql = "insert into mensagemusuario (msgid,usucpf) values ($ultimomsgid,'$dono')";
                			$saida=$db->executar($sql);
		 					$usuemails .= $emaildono.',';  
                	  
                		}
                		// envia msg por email
						if ($usuemails)
						{
							// envia email 
							email_pessoal('Simec','Simec', $usuemails, 'Acompanhamento de atividade em Projeto', "Acompanhar a atividade $descricao no Projeto $pjedsc", '','');
						}
						$db->commit();                	
                 	}
                }
			}
		 		
			}
		}
		


<JSCode>
	<?=$jsCode?>
</JSCode>
<? $db -> close(); exit(); ?>


