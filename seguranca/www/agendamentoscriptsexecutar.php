<?
if (date('i') == '05' || isset($_GET['skip']))
{
	define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../' ) );
	
	// carrega as funções gerais
	require_once BASE_PATH_SIMEC . "/global/config.inc";
	require_once APPRAIZ . "includes/classes_simec.inc";
	require_once APPRAIZ . "includes/funcoes.inc";
	require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
	
	/* configurações */
	set_time_limit(0);
	ini_set("memory_limit", "3000M");
	/* FIM configurações */
	
	$log = null;
	
	$cancel = null;
	
	$queue = new ZendJobQueue();
	
	$list = $queue->getJobsList(array('status' => $queue::JOB_STATUS_RUNNING));
	
	$horainicio = date("d/m/Y H:i:s");
	
	if (count($list) > 0)
	{
		foreach ($list as $job) 
		{
			$today = time();
			$start = strtotime($job['start_time']);
			$diff = (int) round(abs($today-$start)/60/60);
			
			if ($diff >= 12)
			{
				$cancel .= "# Script cancelado: {$job['script']} ({$job['start_time']})<br/>";
				$queue->removeJob($job['id']);
			}
		}
		
		$horatermino = date('d/m/Y H:i:s');
	}
	
	if ($cancel)
	{
		$log .= "<i><b>Inicio da suspensão dos scripts: {$horainicio}</b></i><br>";
		$log .= $cancel;
		$log .= "<i><b>Termino da suspensão dos scripts: {$horatermino}</b></i><br/><br/>";
	}
	
	function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);} 
	 
	$inicio = getmicrotime();
	
	$URI = IS_PRODUCAO ? 'vm-simec-lap-05.mec.gov.br' : $_SERVER['SERVER_NAME'];
	
	// CPF do administrador de sistemas
	if(!$_SESSION['usucpf']) {
		$_SESSION['usucpforigem'] = '';
		$_SESSION['usucpf'] = '';
	}
	
	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	$tm = time();
	
	$datetime = date("Y-m-d H:i:s");
	
	$horainicio = date("d/m/Y H:i:s");

	$sql = "SELECT 
			  	agsid,
			  	agsfile,
			  	agsperiodicidade,
			  	agsperdetalhes,
			  	agsstatus,
			  	agsdataexec,
			  	agsdescricao,
			  	agstempoexecucao,
			    case when agsperiodicidade = 'diario' then
				    	to_char(agsdataexec, 'YYYY-MM-DD HH24') 
			        when agsperiodicidade = 'semanal' then
			        	(SELECT to_char(now(), 'YYYY')||'-'|| EXTRACT(WEEK FROM agsdataexec))
			        when agsperiodicidade = 'mensal' then
			        	to_char(agsdataexec, 'YYYY-MM-DD')
			    else '' end as dataexecucao
			FROM 
			  	seguranca.agendamentoscripts
			WHERE agsstatus='A' ORDER BY agstempoexecucao";
	
	$agendamentos = $db->carregar($sql);
	
	$jobs = array();
	
	if($agendamentos[0]) {
		foreach($agendamentos as $agen) {
			switch($agen['agsperiodicidade']) {
				case 'diario':
					$diahor = explode(";",$agen['agsperdetalhes']);
					if(in_array(date("H"), $diahor) && $agen['dataexecucao'] != date("Y-m-d H") ) {
						$jobs[$agen['agsid']] = $agen['agsfile'];
						$sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec = '{$datetime}' WHERE agsid='".$agen['agsid']."';";
					}
				break;
				case 'semanal':
					if(substr($agen['agsdataexec'],0,10) != date("Y-m-d") && $agen['dataexecucao'] != date("Y-W") ) {
						$diasem = explode(";",$agen['agsperdetalhes']);
						if( in_array(date("w"), $diasem) ) {
							$jobs[$agen['agsid']] = $agen['agsfile'];
							$sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec = '{$datetime}' WHERE agsid='".$agen['agsid']."';";
						}
					}
				break;
				case 'mensal':
					if($agen['dataexecucao'] != date("Y-m-d")) {
						$diamen = explode(";",$agen['agsperdetalhes']);
						if( in_array(date("d"), $diamen) ) {
							$jobs[$agen['agsid']] = $agen['agsfile'];
							$sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec = '{$datetime}' WHERE agsid='".$agen['agsid']."';";
						}
					}
				break;
			}
		}
	}
	
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "Agendamento de scripts - Iniciando scripts";
	$mensagem->From 		= "simec@mec.gov.br";
	$mensagem->AddAddress("alexandre.dourado@mec.gov.br", "Alexandre Dourado");
	$mensagem->Subject = "Agendamento de scripts";
	
	ob_start();
	echo "<pre>";
	print_r($jobs);
	print_r($_SERVER);
	$dadosserv = ob_get_contents();
	ob_end_clean();
	
	$mensagem->Body .= date("d/m/Y H:i:s")."----".$dadosserv."<br/>";
	$mensagem->IsHTML( true );
	$mensagem->Send();

	$log .= "<i><b>Inicio da execução dos scripts: {$horainicio}</b></i><br>";
	
	if ($jobs) {
		foreach($jobs as $agsid => $file) {
			$pos = strpos($file, '?');			
			if ( !empty($pos) ) {
				$files = substr($file, 0, $pos);
			} else {
				$files = $file;
			}
			if(is_file('./scripts_exec/'.$files)) {
				$microtime = getmicrotime();
				
				$url = 'http://' . $URI . '/seguranca/scripts_exec/' . $file;
				$options = array('name' => substr($file, 0, 30), 'schedule_time' => date("Y-m-d H:i:s", strtotime("+1 minute")));
				$queue->createHttpJob($url, array(), $options);
				
				$log .= "# Script executado: {$url}<br/>";
			} else {
				$log .= "Não foi encontrado o arquivo '".$file."'<br/>";
			}
		}
	} else {
		$log .= "Nenhum agendamento encontrados<br/>";
	}
	
	if ($sqls) {
		$db->executar(implode("",$sqls));
		$db->commit();
		$log .= "Atualizações efetuadas com sucesso<br/>";
	} else {
		$log .= "Nenhuma atualização efetuada<br/>";
	}
	
	$horatermino = date('d/m/Y H:i:s');
	
	$log .= "<i><b>Termino da execução dos scripts: {$horatermino}</b></i><br/>";
	
	/*
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = "localhost";
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName		= "Agendamento de scripts";
	$mensagem->From 		= "simec@mec.gov.br";
	$mensagem->AddAddress("alexandre.dourado@mec.gov.br", "Alexandre Dourado");
	$mensagem->AddAddress("danielbrito@mec.gov.br", "Daniel Brito");
	$mensagem->AddAddress("fellipesantos@mec.gov.br", "Fellipe Esteves");
	$mensagem->Subject = "Agendamento de scripts";
	$mensagem->Body .= "Envio da mensagem em ".(getmicrotime() - $inicio)." segundos<br/><br/>";
	$mensagem->Body .= $log;
	
	ob_start();
	echo "<pre>";
	print_r($jobs);
	$dadosserv = ob_get_contents();
	ob_end_clean();
	
	$mensagem->Body .= $dadosserv."<br/>";
	$mensagem->IsHTML( true );
	$mensagem->Send();
	
	/*
	 * FIM
	 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
	 */	
	
	$db->close();
}
?>